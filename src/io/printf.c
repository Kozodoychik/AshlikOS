#include <io/printf.h>
#include <console.h>
#include <stdarg.h>

char convert_buffer[9] = {};
wchar_t wconvert_buffer[9]= {};

void clean_convert_buffer(){

        for (uint8_t i=0; i<9; i++){
                convert_buffer[i] = 0;
                wconvert_buffer[i] = 0;
        }
        convert_buffer[0] = '0';
        wconvert_buffer[0] = L'0';

}

char* tohexstring(int hex){
        clean_convert_buffer();
        char* result = convert_buffer;
        char temp[sizeof(int)*2] = {};
        char hex_table[16] = "0123456789ABCDEF";
        uint8_t offset = 0;
        while (hex > 0){
                uint8_t dig = hex & 0xf;
                temp[offset] = hex_table[dig];
                hex = hex >> 4;
                offset++;
        }
        for (int i=0;i<offset;i++){
                result[i] = temp[offset-i-1];
        }
        return result;
}

char* todecstring(int num){
        clean_convert_buffer();
        char* result = convert_buffer;
        char temp[sizeof(int)] = {};
        char dec_table[10] = "0123456789";
        uint8_t offset = 0;
        while (num > 0){
                uint8_t dig = num % 10;
                temp[offset] = dec_table[dig];
                num /= 10;
                offset++;
        }
        for (int i=0;i<offset;i++){
                result[i] = temp[offset-i-1];
        }
        return result;
}


void printf(char* str, ...){

        console_device* active_console = get_active_console();

        va_list args;
        va_start(args, str);

        uint8_t i=0;
        while (str[i] != '\0'){
                if (str[i] == '%' && str[i+1] == 'X'){
                        printf("0x");
                        printf(tohexstring(va_arg(args, int)));
                        i++;
                }
                else if (str[i] == '%' && str[i+1] == 'd'){
                        printf(todecstring(va_arg(args, int)));
                        i++;
                }
                else if (str[i] == '%' && str[i+1] == 'b'){
                        printf(va_arg(args, int) ? "true" : "false");
                        i++;
                }
                else if (str[i] == '%' && str[i+1] == 'c'){
                        char c = (char)va_arg(args, int);
                        active_console->write(c);
                        i++;
                }
                else{
                        active_console->write(str[i]);
                }
                i++;
        }

        return;

}

void wprintf(wchar_t* str, ...){

        console_device* active_console = get_active_console();

        va_list args;
        va_start(args, str);

        uint8_t i=0;
        while (str[i] != L'\0'){
                if (str[i] == L'%' && str[i+1] == L'X'){
                        wprintf(tohexstring(va_arg(args, int)));
                        i++;
                }
                else if (str[i] == L'%' && str[i+1] == L'd'){
                        wprintf(todecstring(va_arg(args, int)));
                        i++;
                }
                else if (str[i] == L'%' && str[i+1] == L'b'){
                        wprintf(va_arg(args, int) ? L"true" : L"false");
                        i++;
                }
                else if (str[i] == L'%' && str[i+1] == L'c'){
                        wchar_t c = (wchar_t)va_arg(args, int);
                        active_console->write(c);
                        i++;
                }
                else{
                        active_console->write(str[i]);
                }
                i++;
        }

        return;

}
