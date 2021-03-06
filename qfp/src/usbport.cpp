/*
*
* Copyright (C)2013, Samuel Isuani <sisuani@gmail.com>
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
*
* Redistributions in binary form must reproduce the above copyright
* notice, this list of conditions and the following disclaimer in the
* documentation and/or other materials provided with the distribution.
*
* Neither the name of the project's author nor the names of its
* contributors may be used to endorse or promote products derived from
* this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
* HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

#include <QtGlobal>

#include "usbport.h"

#include <QDebug>


UsbPort::UsbPort(const quint16 vid, const quint16 pid)
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    m_usbPort = new QUsbDevice();
    //m_usbPort->setDebug(true);

    m_filter.pid = pid;
    m_filter.vid = vid;

    //
    m_config.alternate = 0;
    m_config.config = 1;
    m_config.interface = 0;
    m_config.readEp = 0x82;
    m_config.writeEp = 0x01;

    QtUsb::DeviceStatus ds;
    ds = m_usbManager.openDevice(m_usbPort, m_filter, m_config);
    m_open = (ds == QtUsb::deviceOK) ? true : false;
#endif
}

bool UsbPort::isOpen()
{
    return m_open;
}

void UsbPort::close()
{
#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    m_usbManager.closeDevice(m_usbPort);
#endif
}

const qreal UsbPort::write(const QByteArray &data)
{
    if (!m_open) return 0;

#ifdef DEBUG
    log() << QString("UsbPort::write() %1").arg(data.toHex());
#endif

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    const qreal size = m_usbPort->write(&data, data.size());
    m_usbPort->flush();
    return size;
#endif

    return 0;
}

QByteArray UsbPort::read(const qreal size)
{
    if (!m_open) return tread;

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    m_usbPort->read(&tread ,size);
    return tread;
#endif

    return 0;
}

QByteArray UsbPort::readAll()
{
    return read(128);
}

const qreal UsbPort::bytesAvailable()
{
    return 0;
}
