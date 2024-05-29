//
// Copyright 2024 Huawei Cloud Computing Technologies Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#ifndef TEST_UTIL_TEST_MOCKIHTTPCLIENT_HPP
#define TEST_UTIL_TEST_MOCKIHTTPCLIENT_HPP

#include <gmock/gmock.h>

#include "opengemini/impl/http/IHttpClient.hpp"

namespace opengemini::test {

using namespace std::chrono_literals;

class MockIHttpClient : public impl::http::IHttpClient {
public:
    MockIHttpClient(boost::asio::io_context&  ctx,
                    std::chrono::milliseconds connectTimeout   = 10s,
                    std::chrono::milliseconds readWriteTimeout = 10s) :
        impl::http::IHttpClient(ctx, connectTimeout, readWriteTimeout)
    { }

    MOCK_METHOD(impl::http::Response,
                SendRequest,
                (const Endpoint&,
                 impl::http::Request,
                 boost::asio::yield_context),
                (override));
};

} // namespace opengemini::test

#endif // !TEST_UTIL_TEST_MOCKIHTTPCLIENT_HPP
