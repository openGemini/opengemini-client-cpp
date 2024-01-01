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

#ifndef OPENGEMINICPP_CLIENT_HPP
#define OPENGEMINICPP_CLIENT_HPP

#include <memory>

namespace opengeminicpp {

class Client {
public:
    Client();
    ~Client();

private:
    class ClientImpl;
    std::unique_ptr<ClientImpl> impl;
};

} // namespace opengeminicpp

#ifndef OPENGEMINICPP_SEPERATE_COMPILATION
#    include "opengeminicpp/impl/client.cpp"
#endif // !OPENGEMINICPP_SEPERATE_COMPILATION

#endif // OPENGEMINICPP_CLIENT_HPP
