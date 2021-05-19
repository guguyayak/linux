# 字符串替换
- 1. sed替换的基本语法为:
> sed 's/原字符串/替换字符串/'   
> 示例  
> echo a_b_c | sed 's/\_/./g'  
> 输出：a.b.c  
> echo a_b_c | sed 's/_/./'  
> 输出：a.b_c  
