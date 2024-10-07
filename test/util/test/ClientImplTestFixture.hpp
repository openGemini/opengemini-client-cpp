// Copyright 2024 openGemini Authors
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

#ifndef TEST_UTIL_TEST_CLIENTIMPLTESTFIXTURE_HPP
#define TEST_UTIL_TEST_CLIENTIMPLTESTFIXTURE_HPP

#include <gtest/gtest.h>

#include "opengemini/ClientConfigBuilder.hpp"
#include "opengemini/impl/ClientImpl.hpp"
#include "test/HackMember.hpp"
#include "test/MockIHttpClient.hpp"

namespace opengemini::test {

using namespace opengemini::impl;

OPENGEMINI_TEST_MEMBER_HACKER(ClientImpl,
                              &ClientImpl::ctx_,  // 0
                              &ClientImpl::http_, // 1
                              &ClientImpl::lb_)   // 2

class ClientImplTestFixture : public testing::Test {
protected:
    ClientImplTestFixture() :
        impl_(ClientConfigBuilder()
                  .AppendAddress({ "127.0.0.1", 1234 })
                  .AppendAddress({ "127.0.0.1", 4321 })
                  .Finalize())
    {
        auto  hackImpl = HackingMember(impl_);
        auto& ctx_     = impl_.*(std::get<0>(hackImpl));
        mockHttp_      = std::make_shared<MockIHttpClient>(ctx_());
        impl_.*(std::get<1>(hackImpl)) = mockHttp_;
    }

protected:
    ClientImpl                       impl_;
    std::shared_ptr<MockIHttpClient> mockHttp_;
};

} // namespace opengemini::test

#endif // !TEST_UTIL_TEST_CLIENTIMPLTESTFIXTURE_HPP
