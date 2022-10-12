/* Copyright (c) 2012, Corentin Jabot. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

  * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

  * Neither the name of the author nor the names of its contributors may
    be used to endorse or promote products derived from this software
    without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "webphandler.h"
#include "webp/decode.h"
#include <QImage>
#include <QDebug>

// 'RIFF'
const quint8 KRiffHeader[4] = {0x52, 0x49, 0x046, 0x46};
// 'WEBPVP8'
const quint8 KWebPHeader[7] = {0x57, 0x45, 0x042, 0x50, 0x56, 0x50, 0x38};

WebPHandler::WebPHandler()
{
}

bool WebPHandler::canRead () const
{
	return canRead(device());
}

bool WebPHandler::canRead( QIODevice *device )
{
	if (!device) {
		qWarning("WebPHandler::canRead() called with no device");
		return false;
	}

	char buffer[16];
	if (device->peek(buffer, 16) != 16)
		return false;

	if(memcmp(buffer, KRiffHeader, 4) == 0 && memcmp(buffer + 8, KWebPHeader, 7) == 0) {
		return true;
	}
	return false;
}

bool WebPHandler::read ( QImage * image )
{
    if(!canRead()) {
        return false;
    }

    QByteArray data = device()->readAll();
    int width = 0, height = 0;
	if(WebPGetInfo((const uint8_t*)data.constData(), data.size(), &width, &height)) {
		QImage result(width, height, QImage::Format_ARGB32);
		uint8_t *output = result.bits();
		size_t output_size = result.byteCount();
		if(WebPDecodeBGRAInto((const uint8_t*)data.constData(), data.size(), output, output_size, result.bytesPerLine())) {
			*image = result;
			return true;
		}
	}

    return false;
}
