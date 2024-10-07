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

#ifndef TEST_UTIL_TEST_SELFROOTCA_HPP
#define TEST_UTIL_TEST_SELFROOTCA_HPP

namespace opengemini::test {

//
// The self-signed root certificate here is NOT intended to make trustworthy
// but just for TEST. It was generated using OpenSSL with the following command:
// $ openssl req -x509                                                      \
//               -newkey rsa:1024                                           \
//               -keyout key.pem                                            \
//               -out selfRootCA.pem                                        \
//               -sha256 -days 36500                                        \
//               -nodes -subj "/C=CN/ST=TEST/L=TEST/O=TEST/OU=TEST/CN=TEST"
//
constexpr auto selfRootCA = R"(-----BEGIN CERTIFICATE-----
MIICjjCCAfegAwIBAgIUJLIn6Uq2LxXXTWkC2C1Aiq85Wp0wDQYJKoZIhvcNAQEL
BQAwWDELMAkGA1UEBhMCQ04xDTALBgNVBAgMBFRFU1QxDTALBgNVBAcMBFRFU1Qx
DTALBgNVBAoMBFRFU1QxDTALBgNVBAsMBFRFU1QxDTALBgNVBAMMBFRFU1QwIBcN
MjQwNjA2MTUwMTM2WhgPMjEyNDA1MTMxNTAxMzZaMFgxCzAJBgNVBAYTAkNOMQ0w
CwYDVQQIDARURVNUMQ0wCwYDVQQHDARURVNUMQ0wCwYDVQQKDARURVNUMQ0wCwYD
VQQLDARURVNUMQ0wCwYDVQQDDARURVNUMIGfMA0GCSqGSIb3DQEBAQUAA4GNADCB
iQKBgQC3I2XKtZ41z4JsIUPm+R++7D0sWsp7aLgI5FdqssFCpLlncGLXzkrMma2T
iRGPAJ4iQL1fMFuBEue8+OUgIubCWGCNnZMKU4SGJpjc3zSlI0cbjkYtFZDf9/TZ
AWZKixSJtspp00GGcZiJDbUzmnyQWix302EvCvTE3ELWp1h92QIDAQABo1MwUTAd
BgNVHQ4EFgQU1GzEH+no161rzdG9XIIi3y4U+/4wHwYDVR0jBBgwFoAU1GzEH+no
161rzdG9XIIi3y4U+/4wDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOB
gQCV8JuPtkOgKkrtzyni2IA4VnQiRWSiwY9jGqiABQbKxVy3Xg0bD7rvMC60crgU
mTSXCoUhi5gaOOW/mOPTgc1tl4vHzsjkDw12EG/LpPgVr2rbvG0vYlRGi8fZy4tg
WX1QVD78v+hdckogsqM6oHnqAXcok2GX9ymISD4z+ftMLg==
-----END CERTIFICATE-----)";

} // namespace opengemini::test

#endif // !TEST_UTIL_TEST_SELFROOTCA_HPP
