#! /usr/bin/env python
# -*- coding: utf-8 -*-

import subprocess
import cStringIO
import re
import os
import sys
import codecs
import string

def _getProjects(qmake_out):
    yield os.getcwdu()
    seen = set()

    re1 = re.compile('^ *Reading (.*)\/.*\.pro\r?$')
    re2 = re.compile('^ *Reading .*\/.*\.pro \[(.*)\]\r?$')
    for line in cStringIO.StringIO(qmake_out):
        ma = re1.search(line)
        if not ma:
            ma = re2.search(line)

        if ma:
            normalized = os.path.normcase(ma.group(1))
            if normalized not in seen:
                seen.add(normalized)
                yield normalized

def _cleanup(lines):
    return [line.rstrip() for line in lines]

def _loadFile(path, modifier):
    return modifier(map(string.rstrip, codecs.open(path, 'r', 'gbk')))

def _saveFile(path, content):
    with codecs.open(path, 'w', 'utf-8') as f:
        f.write('\r\n'.join(content))

def _cure_vcxproj(filelines):
    def repl(s, regs):
        l = s
        for r, p in regs:
            l = r.sub(p, l)

        return l

    regexps = (
        (re.compile('^(\s*)<PropertyGroup Label="Globals">$'), '\g<0>\r\n\\1\\1<PlatformToolset>v140_xp</PlatformToolset>'),
        (re.compile('MOC (\S*) &amp; Combined'), 'MOCombined \\1'),
        (re.compile('MOC (\S*), Combined'), 'MOCombined \\1'),
        (re.compile('^(\s*)<(ResourceOutputFileName)>\S+\/\$\(InputName\)(.res<\/\\2>)$'), '\\1<\\2>$(OutDir)%(Filename)\\3'),
    )

    return map(lambda s: repl(s, regexps), filelines)

def _cure_vcxproj_filters(filelines):
    return filelines

def _cure_sln(filelines):
	startIndex = 2
	if ('# Visual Studio' not in filelines[1]):
		startIndex = 1
	
	for i, line in enumerate(filelines):
		if ("ConfigName.0 = Debug|Win32" in line):
			filelines[i] = line.replace("ConfigName.0", "Debug|Win32")
			
		if ("ConfigName.1 = Release|Win32" in line):
			filelines[i] = line.replace("ConfigName.1", "Release|Win32")
			
		if ("ConfigName.0 = Debug|x64" in line):
			filelines[i] = line.replace("ConfigName.0", "Debug|x64")
			
		if ("ConfigName.1 = Release|x64" in line):
			filelines[i] = line.replace("ConfigName.1", "Release|x64")
	
		if ("GlobalSection(SolutionConfiguration)" in line):
			filelines[i] = line.replace("SolutionConfiguration", "SolutionConfigurationPlatforms")
			
		if ("GlobalSection(ProjectConfiguration)" in line):
			filelines[i] = line.replace("ProjectConfiguration", "ProjectConfigurationPlatforms")

	
	return ['Microsoft Visual Studio Solution File, Format Version 14.00', '# Visual Studio 2015'] + [line for line in filelines][startIndex:]
	

def _cure_path(path):
    doctors = (
        ('.vcxproj', _cure_vcxproj),
        ('.vcxproj.filters', _cure_vcxproj_filters),
        ('.sln', _cure_sln),
    )
    for root, dirs, files in os.walk(path):
        for f in files:
            for s, p in doctors:
                if f.endswith(s):
                    fp = os.path.join(root, f)
                    print 'Curing ' + fp
                    _saveFile(fp, _loadFile(fp, p))
                    break
        break;

def main():
    command_list = ['qmake', '-tp', 'vc', '-r'] + sys.argv[1:]

    print u'Running qmake @ ' + os.getcwdu()
    process = subprocess.Popen(
        command_list,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT)

    for proj in _getProjects(process.communicate()[0]):
        _cure_path(proj)

if __name__ == '__main__':
    main() 

