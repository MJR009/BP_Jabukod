grammar Jabukod;

@parser::members {
private:
    int col;
public:
    JabukodParser(antlr4::TokenStream *input, int col) : JabukodParser(input, antlr4::atn::ParserATNSimulatorOptions()) {
        this->col = col;
    }
}

file
    : (row NL)+
    ;

row
locals [int i=0]
    :
    (
        ENTRY {
$i++;
if ($i == col) {
    std::cout << $ENTRY.text << std::endl;
}
        }
    )+
    ;

ENTRY
    : (~[ \t\r\n])+
    ;

NL
    : '\r'? '\n'
    ;

TAB
    : [ \t] -> skip
    ;