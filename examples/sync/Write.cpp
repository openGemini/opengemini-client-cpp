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
// Example: Write points synchronously.
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
        // Writes single point to server, the operation will
        // block until completes or an exception is thrown.
        client.Write("ExampleDatabase",
                     {
                         "ExampleMeasurement",
                         {
                             { "Weather", "sunny" },
                             { "Humidity", 521 },
                             { "Temperature", 38.1 },
                         },
                         std::chrono::system_clock::now(),
                     });
    }

    {
        // Constructs two points for writing.
        opengemini::Point point1{
            "ExampleMeasurement",
            {
                { "Weather", "sunny" },
                { "Humidity", 521 },
                { "Temperature", 38.1 },
            },
        };
        opengemini::Point point2{
            "ExampleMeasurement",
            {
                { "Weather", "rainy" },
                { "Humidity", 333 },
                { "Temperature", 36.5 },
            },
        };

        // Writes multiple points to server, the operation will
        // block until completes or an exception is thrown.
        client.Write("ExampleDatabase",
                     { std::move(point1), std::move(point2) });
    }

    {
        // Performs a write request which will fail.
        try {
            client.Write("ExampleDatabase",
                         {
                             "ExampleMeasurement",
                             {}, // Passing an empty fields map is not allowed
                             std::chrono::system_clock::now(),
                         });
        }
        catch (const opengemini::Exception& exception) {
            // Simply print the exception information.
            std::cout << exception << std::endl;
        }
    }

    return 0;
}
