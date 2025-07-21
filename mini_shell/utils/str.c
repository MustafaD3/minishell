#include "../minishell.h"

char	*string_concatation(char **str)
{
	char	*result;
	int		total_byte;
	int		i;
	
	if (!str)
		return (NULL);
		
	i = 0;
	total_byte = 0;
	while(str[i]){
		if (str[i])  // NULL kontrolü ekle
			total_byte+= ft_strlen(str[i]);
		i++;
	}
	result = malloc(total_byte + 1);
	if(!result)
		return (NULL);
		
	i = 0;
	total_byte = 0;
	while(str[i]){
		if (str[i])  // NULL kontrolü ekle
		{
			ft_memcpy(result + total_byte, str[i], ft_strlen(str[i]));
			total_byte += ft_strlen(str[i]);
		}
		i++;
	}
	result[total_byte] = '\0';
	return (result);
}
