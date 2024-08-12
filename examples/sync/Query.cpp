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

//
// Example: Query from server synchronously.
//

#include <iostream>

#include <opengemini/Client.hpp>
#include <opengemini/ClientConfigBuilder.hpp>
#include <opengemini/Exception.hpp>

int main(int argc, char** argv)
{
    // Constructs an openGemini client object.
    opengemini::Client client{ opengemini::ClientConfigBuilder()
                                   // At least one server endpoint needed.
                                   .AppendAddress({ "127.0.0.1", 8086 })
                                   .Finalize() };

    {
        // Performs a query request and print the result, the operation will
        // block until completes or an exception is thrown.
        auto result = client.Query({
            "ExampleDatabase",
            "select * from ExampleMeasurement",
        });
        std::cout << result << std::endl;
    }

    {
        // Performs a query which will fail.
        try {
            opengemini::QueryResult result = client.Query({
                "ExampleDatabase",
                {}, // Passing an empty <command> is not allowed
            });
        }
        catch (const opengemini::Exception& exception) {
            // Simply prints the exception information.
            std::cout << exception << std::endl;
        }
    }

    return 0;
}
