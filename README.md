# Nice Simple Multi-Algorithm mining

C function that gets profitability information about algorithms and returns back most profitable algorithm.

Should be compatible with all cpuminer based projects (ccminer, sgminer, cgminer,...).

In some cases, you may modify your json_rpc_call function usually located in util.c; simply add:
curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);

Algorithm names: https://www.nicehash.com/index.jsp?p=simplemultialgo#names