#include <stdio.h>

typedef long long t_ll;

t_ll	get_llmin(void)
{
	t_ll	ll_min;

	ll_min = 1;
	while (ll_min > 0)
		ll_min <<= 1;
	return (ll_min);
}

t_ll	ft_atoll(const char *s)
{
	int		neg_flag;
	int		i;
	t_ll	res;

	res = 0;
	neg_flag = 0;
	i = 0;
	while ((s[i] > 8 && s[i] < 14) || s[i] == 32)
		i++;
	if (s[i] == '-' || s[i] == '+')
	{
		if (s[i] == '-')
			neg_flag = 1;
		i++;
	}
	while (s[i] > 47 && s[i] < 58)
	{
		res = (res * 10) + (s[i] - '0');
		i++;
	}
	if (neg_flag)
		res *= -1;
	return (res);
}

int main(void)
{
	printf("%lld\n", get_llmin());

	printf("%lld\n", ft_atoll("-9223372036854775808"));
	printf("%lld\n", ft_atoll("-884295885767384"));
	printf("%lld\n", ft_atoll("-1024"));
	printf("%lld\n", ft_atoll("0"));
	printf("%lld\n", ft_atoll("1024"));
	printf("%lld\n", ft_atoll("884295885767384"));
	printf("%lld\n", ft_atoll("9223372036854775807"));

	return (0);
}
