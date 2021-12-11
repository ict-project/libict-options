typedef std::vector<std::string> tokens_t;
static bool tokenizer(const char & c){
    if (std::isalnum(c)) return true;
    switch (c){
        case '-':case '_':case '.':return true;
        default:break;
    }
    return false;
}
static void tokenizer(const std::string & input,tokens_t & tokens){
    std::string token;
    for (const char & c : input) {
        if (tokenizer(c)){
            token+=c;
        } else {
            if (!token.empty()) tokens.push_back(token);
            token.clear();
        }
    }
    if (!token.empty()) tokens.push_back(token);
}