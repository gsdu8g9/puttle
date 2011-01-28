/*
 * Copyright (C) 2011 Camille Moncelier
 *
 * This file is part of puttle.
 *
 * puttle is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * puttle is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with puttle in the COPYING.txt file.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <proxy.h>

#include <string>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

namespace puttle {
Proxy::Proxy() :
    port(3128),
    host("") {
}

Proxy::Proxy(std::string host_, uint16_t port_,
             std::string username_, std::string password_) :
    port(port_),
    host(host_),
    username(username_),
    password(password_) {
}

Proxy Proxy::parse(std::string url) {
    static boost::regex r("(?:(?:(\\w+):)?(?://))?(?:(?:(?:([^:@/]*):?([^:@/]*))?@)?([^:/?#]*)(?::(\\d*))?)?(\\.\\.?$|(?:[^?#/]*/)*)([^?#]*)(?:\\?([^#]*))?(?:#(.*))?$");
    boost::smatch match;
    if (regex_match(url, match, r)) {
        Proxy p;
        p.username = match[2];
        p.password = match[3];
        p.host = match[4];
        try {
            if (match[5].matched) {
                p.port = boost::lexical_cast<uint16_t>(match[5]);
            }
        } catch(const boost::bad_lexical_cast& e) {
            return invalid_proxy;
        }

        return p;
    }
    return invalid_proxy;
}

bool Proxy::is_valid() {
    return !host.empty();
}

const Proxy Proxy::invalid_proxy = Proxy();
}

