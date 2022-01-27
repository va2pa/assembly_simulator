#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include<headers/common.h>

uint64_t string2uint(const char *str)
{
    return string2uint_range(str, 0, -1);
}

uint64_t string2uint_range(const char *str, int start, int end)
{
    end = end == -1 ? strlen(str) - 1 : end;
    int state = 0;
    uint64_t uv = 0;
    // 负数标记
    int sign_bit = 0;
    for(int i = start; i <= end; i++)
    {
        if(state == 0)
        {
            if(str[i] == ' ')
            {

            }
            if(str[i] == '0')
            {
                state = 1;
            }
            else if(str[i] >= '1' && str[i] <= '9')
            {
                state = 2;
                
            }
            else if(str[i] == '-')
            {
                state = 3;
            }
            else{ goto fail; }
        }
        else if(state == 1)
        {
            if(str[i] == 'x'){
                state = 4;
            }
            else if(str[i] >= '0' && str[i] <= '9')
            {
                state = 2;
            }
            else if(str[i] == ' ')
            {
                state = 6;
            }
            else{ goto fail; }
        }
        else if(state == 2)
        {
            if(str[i] >= '0' && str[i] <= '9')
            {
                
            }
            else if(str[i] == ' ')
            {
                state = 6;
            }
            else{ goto fail; }
        }
        else if(state == 3)
        {
            if(str[i] == '0'){
                state = 1;
            }
            else if(str[i] >= '1' && str[i] <= '9')
            {
                state = 2;
            }
            else{ goto fail; }
        }
        else if(state == 4)
        {
            if((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'f'))
            {
                state = 5;
            }
            else{ goto fail; }
        }
        else if(state == 5)
        {
            if((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'f'))
            {

            }
            else if(str[i] == ' ')
            {
                state = 6;
            }
            else{ goto fail; }
        }else if(state == 6)
        {
            if(str[i] == ' ')
            {

            }
            else{ goto fail; }
        }

        
        //计算结果
        if(state == 2){
            uint64_t old_val = uv;
            uv = uv * 10 + str[i] - '0';
            if(old_val > uv){
                // overflow
                printf("%s overflow", str);
                goto fail;
            }
        }
        else if(state == 3)
        {
            sign_bit = 1;
        }
        else if(state == 5)
        {
            uint64_t old_val = uv;
            if(str[i] >= '0' && str[i] <= '9')
            {
                uv = uv * 16 + str[i] - '0';
            }
            else
            {
                uv = uv * 16 + str[i] - 'a' + 10;
            }
            if(old_val > uv){
                // overflow
                printf("%s overflow", str);
                goto fail;
            }
        }
    }
    if(sign_bit == 1){
        //无符号最高位为1其他位为0时，它的负数即最小有符号数
        // uint8_t uv = 0x80;
        // int8_t v = -1 * uv == 0x80 == -128
        if((uv & 0x8000000000000000) != 0 &&
            (uv & 0x7fffffffffffffff) != 0)
        {
            printf("%s overflow:unsigned can't convert to signed\n", str);
            exit(0);
        }
        //返回内存布局
        return ~uv + 1;
    }
    return uv;

    fail: 
    printf("fail\n");
    exit(0);
}