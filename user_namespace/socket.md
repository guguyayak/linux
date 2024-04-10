# 地址比较
```c
/**
 * ganesha.nfsd
 * @brief Compare 2 sockaddrs, including ports
 *
 * @param[in] addr_1      First address
 * @param[in] addr_2      Second address
 * @param[in] ignore_port Whether to ignore the port
 *
 * @return Comparator true/false,
 */
int cmp_sockaddr(sockaddr_t *addr_1, sockaddr_t *addr_2, bool ignore_port)
{
	if (addr_1->ss_family != addr_2->ss_family)
		return 0;

	switch (addr_1->ss_family) {
	case AF_INET:
	{
		struct sockaddr_in *inaddr1 = (struct sockaddr_in *)addr_1;
		struct sockaddr_in *inaddr2 = (struct sockaddr_in *)addr_2;

		return (inaddr1->sin_addr.s_addr == inaddr2->sin_addr.s_addr
			&& (ignore_port ||
			    inaddr1->sin_port == inaddr2->sin_port));
	}
	case AF_INET6:
	{
		struct sockaddr_in6 *ip6addr1 = (struct sockaddr_in6 *)addr_1;
		struct sockaddr_in6 *ip6addr2 = (struct sockaddr_in6 *)addr_2;

		return (memcmp(ip6addr1->sin6_addr.s6_addr,
			       ip6addr2->sin6_addr.s6_addr,
			       sizeof(ip6addr2->sin6_addr.s6_addr)) == 0)
			&& (ignore_port ||
			    ip6addr1->sin6_port == ip6addr2->sin6_port);
	}
	break;
#ifdef RPC_VSOCK
	case AF_VSOCK:
	{
		struct sockaddr_vm *svm1 = (struct sockaddr_vm *)addr_1;
		struct sockaddr_vm *svm2 = (struct sockaddr_vm *)addr_2;


		return (svm1->svm_cid == svm2->svm_cid
			&& (ignore_port || svm1->svm_port == svm2->svm_port));
	}
	break;
#endif /* VSOCK */
	default:
		return 0;
	}
}
```
