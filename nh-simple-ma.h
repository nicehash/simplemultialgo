/// Query profitability of all algorithms on NiceHash and return index of most profitable algorithm.
/// Parameters:
/// - algorithm_names: Array of null terminated strings (should be lower case).
/// - algorithm_factors: Array of factors - speeds.
/// - algorithm_count: Number of algorithms provided in previous two arrays.
/// - opt_port: Optional out parameter that tells port number for most profitable algorithm. Set to NULL if not needed.
/// Return value:
/// Index of most profitable algorithm provided in algorithm_names. It returns ~0 if error occured.
size_t nicehash_simplemultialgo_get(const char** algorithm_names, const double* algorithm_factors, const size_t algorithm_count, int* opt_port)
{
	CURL *curl;
	json_t* val, *data;
	size_t k, i, total;
	double best_prof = 0;
	size_t best_index = ~0;
	int best_port = 0;

	curl = curl_easy_init();
	val = json_rpc_call(curl, "https://www.nicehash.com/api?method=simplemultialgo.info", NULL, "", NULL, 0);
	// note: if this method fails with NULL being returned, then adding following line of code to your 
	// json_rpc_call method will most likely fix it:
	// curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
	// this call may fail sometimes - when NiceHash service is in maintenance mode!
	if (!val) goto end;

	data = json_object_get(val, "result");
	if (!val) goto end;

	data = json_object_get(data, "simplemultialgo");
	if (!val) goto end;

	total = json_array_size(data);
	for (i = 0; i < total; i++)
	{
		const char* alg_name_s;
		json_t* alg_name;
		json_t* alg = json_array_get(data, i);
		if (!alg) goto end;
		alg_name = json_object_get(alg, "name");
		if (!alg_name || !json_is_string(alg_name)) goto end;
		alg_name_s = json_string_value(alg_name);
		for (k = 0; k < algorithm_count; k++)
		{
			if (!strcmp(alg_name_s, algorithm_names[k]))
			{
				const char* alg_prof_s;
				double alg_prof_d;
				json_t* alg_prof = json_object_get(alg, "paying");
				if (!alg_prof || !json_is_string(alg_prof)) goto end;
				alg_prof_s = json_string_value(alg_prof);
				alg_prof_d = atof(alg_prof_s);
				if (algorithm_factors[k] * alg_prof_d > best_prof)
				{
					json_t* alg_port = json_object_get(alg, "port");
					if (!alg_port || !json_is_integer(alg_port)) goto end;
					best_port = (int)json_integer_value(alg_port);
					best_prof = algorithm_factors[k] * alg_prof_d;
					best_index = k;
				}
				break;
			}
		}
	}

end:
	if (opt_port)
		*opt_port = best_port;

	json_decref(val);
	curl_easy_cleanup(curl);

	return best_index;
}