//
// networking.c - some shared networking code
//
// v1.5 / 2022-08-25 / Io Engineering / Terje
//

/*

Copyright (c) 2021-2022, Terje Io
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this
list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its contributors may
be used to endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "networking.h"

#if ETHERNET_ENABLE || WIFI_ENABLE

#include <string.h>

// NOTE: increase #define NETWORK_SERVICES_LEN in networking.h when adding to this array!
PROGMEM static char const *const service_names[] = {
    "Telnet,",
    "Websocket,",
    "HTTP,",
    "FTP,",
    "DNS,",
    "mDNS,",
    "SSDP,",
    "WebDAV,"
};

PROGMEM static const network_services_t allowed_services = {
#if TELNET_ENABLE
    .telnet = 1,
#endif
#if WEBSOCKET_ENABLE
    .websocket = 1,
#endif
#if FTP_ENABLE && SDCARD_ENABLE
    .ftp = 1,
#endif
#if HTTP_ENABLE
    .http = 1,
  #if WEBDAV_ENABLE
    .webdav = 1,
  #endif
#endif
#if DNS_ENABLE
    .dns = 1
#endif
};

network_services_t networking_get_services_list (char *list)
{
    uint_fast8_t idx = 0;
    network_services_t services = {allowed_services.mask};

    while(services.mask) {
        if(services.mask == 1)
            strncat(list, service_names[idx], strlen(service_names[idx]) - 1);
        else
            strcat(list, services.mask & 0x1 ? service_names[idx] : "N/A,");
        idx++;
        services.mask >>= 1;
    }

    return *list != '\0' ? allowed_services : (network_services_t){0};
}

#endif // ETHERNET_ENABLE || WIFI_ENABLE
