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

#ifndef TEST_UTIL_TEST_SELFROOTCA_HPP
#define TEST_UTIL_TEST_SELFROOTCA_HPP

namespace opengemini::test {

constexpr auto selfRootCA = R"(-----BEGIN CERTIFICATE-----
MIICajCCAhGgAwIBAgIUE0IXwsCWFMxSvWYeOKFPyS0QX8MwCgYIKoZIzj0EAwIw
gaIxCzAJBgNVBAYTAkNOMRYwFAYDVQQIDA1UZXN0IFByb3ZpbmNlMRYwFAYDVQQH
DA1UZXN0IExvY2FsaXR5MRowGAYDVQQKDBFUZXN0IE9yZ2FuaXphdGlvbjESMBAG
A1UECwwJVGVzdCBVbml0MRQwEgYDVQQDDAtUZXN0IENvbW1vbjEdMBsGCSqGSIb3
DQEJARYOdGVzdEB0ZXN0LnRlc3QwIBcNMjQwMzIwMTQyMTU1WhgPMjEyNDAyMjUx
NDIxNTVaMIGiMQswCQYDVQQGEwJDTjEWMBQGA1UECAwNVGVzdCBQcm92aW5jZTEW
MBQGA1UEBwwNVGVzdCBMb2NhbGl0eTEaMBgGA1UECgwRVGVzdCBPcmdhbml6YXRp
b24xEjAQBgNVBAsMCVRlc3QgVW5pdDEUMBIGA1UEAwwLVGVzdCBDb21tb24xHTAb
BgkqhkiG9w0BCQEWDnRlc3RAdGVzdC50ZXN0MFkwEwYHKoZIzj0CAQYIKoZIzj0D
AQcDQgAEa68DUSX3y451Xax+koPPPJzNCuNLsSr8OLGbGfXZfw4NsB679bfDDXU5
MDN+NYGbb4NKWH2CNDPAz9kKu/qwcKMhMB8wHQYDVR0OBBYEFCemJ94A8tdF8S2m
oc0vHrg8jO47MAoGCCqGSM49BAMCA0cAMEQCIA55qesHsDB/MmVVQoxzz2/+3O4E
G2J4Xrpbjca5aWIbAiBB4QEL8MF8FdjMUSL5iMt3TfC9kMbOhjqk1CmpOkitPA==
-----END CERTIFICATE-----)";

} // namespace opengemini::test

#endif // !TEST_UTIL_TEST_SELFROOTCA_HPP
