unsigned char hex_to_uchar(const char *hex) {
    unsigned char result = 0;
    while (*hex) {
        result *= 16;
        if (*hex >= '0' && *hex <= '9') {
            result += *hex - '0';
        } else if (*hex >= 'A' && *hex <= 'F') {
            result += *hex - 'A' + 10;
        } else if (*hex >= 'a' && *hex <= 'f') {
            result += *hex - 'a' + 10;
        }
        hex++;
    }
    return result;
}