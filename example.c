void example()
{
	char* anames[] = { "scrypt", "x11", "quark" };
	double factors[] = { 1, 7, 6 };
	int port_out;

	size_t index = nicehash_simplemultialgo_get(anames, factors, 3, &port_out);

	printf("index=%d, port=%d\n", index, port_out);
}