# ascip
c++ ascii not poisonous parser. requires c++23. for examples see a sectoin "examples" below.

the parser was created as struct template, so you can parametrize your method for create grammar by it, instead of include ascip file and depend on it. the ascip needs in tuple for inner use and you can prametrize the structure with any tuple wich have get method in adl. the second template parameter, factory, can to be the void type if you don't want to run the test method.

below i've tried using examples for quick start. all of them leavs in the examples directory in aclual state. also i've tried to use the godbolt, but sometimes it can fail, unfortunatly.
## parser list
here is a list of avaible parsers. you can find examples below
- `int_` an integer if it can to be stored to result
- `d10` or `digit` for parser 0-9
- `lower` and `upper` for parse asci lower or upper letters. and `letter` is a `lower` or `upper`.
- `space` means spaces
- `any` parses any character (if the character is not an ascii store (use `push_back`) it in result)
- `nl` parses new line character
- `quoted_string` parsers string in single or double quoted with escaping character \\. also `dquoted_string` parses only string and double quotes and `squoted_string` - single.
- `char_<'a'>` char with concreate value (it can to be wide char and so on). and `_char<'a'>` is same with omitted value.
- `lit<"string">` is a string literal. please note the string literal right inside the template parameter. unfortunatly it can to be called only with template keyword, or, with same way as terms parsers, but using `sterm` insead of `term` or `tmpl`.
- `operator |` for parse variant
- `lreq<num>` for parse variant with left reqursion (num is position in variant). for other parser for left reqursion see the example below.
- `operator !` for negate parser
- `unary -` for parse optional value. if tere is no value the default constructor will be used.
- `binary -` for parse one value except other
- `*` and `+` for lists
- `%` for parse separated values
- `()` with lambda for semact
- `as` method for treat some parser as value
- `omit` method for skip value
- `cur_pos` just stores to result current position, parse nothing
- `>>` for sequence parser
- `>` for sequence parser. it causes an error if the parser fails with message "unknown" (see must method).
- `check` method checks that the parser got as result exactly required type
- `cast` method try to `static_cast` gotten result to required type. it usefull for parse to struct with inheritance as result due for language limitations. see example below.

with sequence parser can be used
- `cur_shift` for store to result current shift position from sequence start
- `req<number>` for call the sequence (or parent sequence) recursively. the req parser also can be combined with `()` operator with lambda for create recursion result value.
- `++` prefix or postfix for increate result's field number to store parser result. prefix `++` will increate the number for current parser and postfix `++` - for next parser.
- `--` same as `++` but decrease the number
- `finc<number>` method. same as `++` or `--` but you can specify the number will be added to current position (the number can also to be negative).
- `fnum<number>` method. setts the resulting field position independently the current one.
- `must` method. causes an error on fail. accepts message as template parameter, it will be passed to error function, passed in parse method. parameters: result, source on start parser position, current line number, message.
- lambda in sequnce: sequence parser will call it with same arguments as must method. it returned value, if present, will be added to the source position (or causes error if less then 0).

# examples

for parse something we have to provide data source. for ascip it's a class what has `operator+=`, `operator bool` and `operator()`.
- the `operator()` returns symbol and increamets the position.
- the `operator+=` moves the position forward and
- the `operator bool` returns true if there is next symbol
- NOTE: the object has to be light wight: it will be copied many times

`ascip::make_source` can create such object from string_view or string literal.

## simple parser

For example we want to parse version number like 'v1.2'.  the major version is 1 and minor version is 2. code to parse it may looks like
```
struct version {
  int major=0;
  int minor=0;
};

static_assert( ({
    version result;
    parse(_char<'v'> >> int_ >> _char<'.'> >> ++int_, ascip::make_source("v1.2"), result);
// NOTE: -^-----------------------------------^^-----
    result.major + result.minor; }) == 3 );
```

note
- prefix `++` operator. it increases the result field number. first `int_` will store result into `major` and second `int_` stores result into `minor`. for store result to structure instead of single result (for example store to `std::string`) it must to be at least one `++` prefix or postfix operator, or `finc<number>` function for set shift from current field number. wihtout it the result will be treated as single variable.
- `_char` instead of `char_` . the first one omits its value, the second one stores it to result.
- there is no `lexeme()`: we don't use skip parser here. (you can pass it as second argument.)
- static_assert check works only inside constexpr
- [see full example on godbolt](https://godbolt.org/#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:1,endLineNumber:4,positionColumn:1,positionLineNumber:4,selectionStartColumn:1,selectionStartLineNumber:4,startColumn:1,startLineNumber:4),source:'%23include+%3Ctuple%3E%0A%23include+%3Cstring_view%3E%0A%23include+%3Ciostream%3E%0A%0A%23include+%3Chttps://raw.githack.com/zerhud/ascip/master/build/ascip.hpp%3E%0A%0Astruct+version+%7B%0A++int+major%3D0%3B%0A++int+minor%3D0%3B%0A%7D%3B%0A%0Atemplate%3Ctypename+gh,template%3Cauto%3Eclass+term%3Dgh::template+term%3E%0Aconstexpr+auto+make_grammar()+%7B%0A%09return+lexeme(term%3C!'v!'%3E::_char+%3E%3E+gh::int_+%3E%3E+term%3C!'.!'%3E::_char+%3E%3E+%2B%2Bgh::int_)%3B%0A%09//NOTE:%0A%09//++_char+is+same+as+omit(char_)%0A%09//++lexeme+is+a+function+found+by+adl+in+ascip_details+namespace+%0A%09//++%2B%2B+prefix+and+postfix+increases+result!'s+field+number%0A%7D%0A%0Ausing+parser+%3D+ascip%3Cstd::tuple%3E%3B%0A%0Aconstexpr+void+test_grammar()+%7B%0A++static_assert(+(%7B%0A++++version+result%3B%0A++++parse(make_grammar%3Cparser%3E(),+%2Bparser::space,+parser::make_source(%22v1.2%22),+result)%3B%0A++++result.major+/%3D+(result.major+%3D%3D+1)%3B%0A++++result.minor%3B+%7D)+%3D%3D+2+)%3B%0A++//+we+can+also+use+a+string_view+as+a+source%0A++//+the+source+must+to+be+light+weight:+it+will+be+copied+many+times%0A++static_assert(+(%7B%0A++++std::string_view+src+%3D+%22v3.4%22%3B%0A++++version+result%3B%0A++++parse(make_grammar%3Cparser%3E(),+%2Bparser::space,+parser::make_source(%22v3.4%22),+result)%3B%0A++++result.major+/%3D+(result.major+%3D%3D+3)%3B%0A++++result.minor%3B+%7D)+%3D%3D+4+)%3B%0A%7D%0A%0Aint+main(int,char**)+%7B%0A++return+0%3B%0A%7D%0A'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:100,l:'4',m:50,n:'0',o:'',s:0,t:'0'),(g:!((g:!((h:compiler,i:(compiler:g131,deviceViewOpen:'1',filters:(b:'0',binary:'1',binaryObject:'1',commentOnly:'0',debugCalls:'1',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'-std%3Dc%2B%2B23',overrides:!(),selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1),l:'5',n:'0',o:'+x86-64+gcc+13.1+(Editor+%231)',t:'0')),header:(),k:50,l:'4',m:50,n:'0',o:'',s:0,t:'0'),(g:!((h:output,i:(compilerName:'x86-64+gcc+13.1',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x86-64+gcc+13.1+(Compiler+%231)',t:'0')),header:(),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',m:50,n:'0',o:'',t:'0')),l:'3',n:'0',o:'',t:'0')),version:4)

## poison
ok, but what about poison? ascip supports any compatibl type. for example std::vector and std::list can be used for same parser:

```
static_assert( ({
        std::vector<char> r;
        parse(+char_<'a'>, make_source("aaa"), r);
        r.size(); }) == 3 );

std::list<char> r; // list is not constexpr :(
parse(+char_<'a'>, make_source("aaa"), r);
return r.size() != 3;
```

Generally, all types what implements emplace_back and pop_back (or emplace_back and pop_back in adl) can be used as an container.

[example in godbolt](https://godbolt.org/#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:23,endLineNumber:27,positionColumn:23,positionLineNumber:27,selectionStartColumn:23,selectionStartLineNumber:27,startColumn:23,startLineNumber:27),source:'%23include+%3Ctuple%3E%0A%23include+%3Clist%3E%0A%23include+%3Cvector%3E%0A%23include+%3Ciostream%3E%0A%0A%23include+%3Chttps://raw.githack.com/zerhud/ascip/master/build/ascip.hpp%3E%0A%0Atemplate%3Ctypename+gh%3E%0Aconstexpr+auto+make_grammar_a1()+%7B+return+%2Bgh::template+char_%3C!'a!'%3E%3B+%7D%0A%0A//+in+first+make_grammar+we+need+to+write+a+template+keywork+every+time%0A//+or+declare+char_+with+using.+there+is+a+term+struct+in+ascip+and+you%0A//+can+use+it+in+template+declaration+like+this:%0Atemplate%3Ctypename+gh,template%3Cauto%3Eclass+term%3Dgh::template+term%3E%0Aconstexpr+auto+make_grammar_a2()+%7B+return+%2Bterm%3C!'a!'%3E::char_%3B+%7D%0A%0Ausing+parser+%3D+ascip%3Cstd::tuple%3E%3B%0A%0Aint+main(int,char**)+%7B%0A++++static_assert(+(%7B%0A++++++++std::vector%3Cchar%3E+r%3B%0A++++++++parse(make_grammar_a1%3Cparser%3E(),+parser::make_source(%22aaa%22),+r)%3B%0A++++++++r.size()%3B+%7D)+%3D%3D+3+)%3B%0A%0A++++//+the+list+is+not+constexpr+(in+c%2B%2B23)%0A++++//+so+we+cannot+store+the+result+in+it%0A++++std::list%3Cchar%3E+r%3B%0A++++parse(make_grammar_a2%3Cparser%3E(),+parser::make_source(%22aaa%22),+r)%3B%0A++++return+r.size()+!!%3D+3%3B%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:100,l:'4',m:50,n:'0',o:'',s:0,t:'0'),(g:!((g:!((h:compiler,i:(compiler:armg1320,deviceViewOpen:'1',filters:(b:'0',binary:'1',binaryObject:'1',commentOnly:'0',debugCalls:'1',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'-std%3Dgnu%2B%2B2b',overrides:!(),selection:(endColumn:23,endLineNumber:35,positionColumn:23,positionLineNumber:35,selectionStartColumn:23,selectionStartLineNumber:35,startColumn:23,startLineNumber:35),source:1),l:'5',n:'0',o:'+ARM+GCC+13.2.0+(Editor+%231)',t:'0')),header:(),k:50,l:'4',m:50,n:'0',o:'',s:0,t:'0'),(g:!((h:output,i:(compilerName:'x86-64+gcc+13.1',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+ARM+GCC+13.2.0+(Compiler+%231)',t:'0')),header:(),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',m:50,n:'0',o:'',t:'0')),l:'3',n:'0',o:'',t:'0')),version:4)

## reqursion
what about reqursion? we can write a class wrapper with redefined operator = for it can be used to create a value. or we can create reqursion value in lambda. for example let's parse a type like `box<list<string>, int>`. i use a vector of unique_ptr for simplify this example. [please see full example on godbolt](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGe1wAyeAyYAHI%2BAEaYxCAArKQADqgKhE4MHt6%2BekkpjgJBIeEsUTHxdpgOaUIETMQEGT5%2BXLaY9nkM1bUEBWGR0XG2NXUNWc0KQ93BvcX9sQCUtqhexMjsHOYAzMHI3lgA1CYbbuPEwcCH2CYaAIKb27uYB0cAbhVExBdXt2ZbDDte%2B0ObjYLBIAE9Pjcvnc/g8nm4EAQCAkFCAAPRo4hMADuADpgIQEExkABrXFoFhogBe0QQALRTAUyDwCTRLEZBGiaIiXjo6AZTJZuIQCQSkNuUJuJy8Dj2BDBCUeJgA7FZJQBOcboEAgE5nPbMNiHNUSzUEbUgV4OEhArU6rwMPAARy8mAA%2BgkCB8jvLFZ8Ntg9govBE3b7MApjdDlQARKOS66clgJAycoHhw2PYAIUhJlNMNNHJheIgXHaMhRy6IsQ4x7M6vOpx6c4g1gNfNAMcaYVQJYh7YtEPbsknu4BYljs4gQOYHVVfPaLvad7u9/uD1B7PBYQRPGN7eiqTBsaBIEBiBJEp6XAN7ABUp51F6vKrce3rIHQXA0c7fLbbbhgBwbpARcOrIESxBunMczxtcS5BjUjjIG6FbRAQEB7BAKoWIhFp6kYezehYCS1EoEDbowBCkO%2BZ4ju6ChLCsmDYWYZioCS5hmAsRGwRsJoIYuEAfLEFgaCYsT7rWtZ7EBqBAbO1hYWY97CRJFhcBJcYbNpMZASSCl8bhKoxopOkyRsNFcaIDB7KRxBKP2lGCFxRnQjcCErpya4DiWm7hh6e7ueqgmLs5BDWEpGKRZYez8E5fzEJgjKPElwa0AQcV4K06AGn0Hwagh/qBgAtBA/5AqBRxVdgOpuhBtR7BiRGYE6ezYoSWGzqgipYu8NHYo8pEKJWBjFOgTBypuyBJQWzYIKlEZeBlcUkC1LoOWkwVFe2t7ldWQKaQGOpJU62GiVpEAbuYABsxCwaqxC4mlmAYSE2JygqLEwcaSUEMstlPcAr0zlGsaKWYsSybwClNWi7WPDZA60IxLX/cQXYDi1VDRIwqzbUuxVVq2lXrAGNV1Q190E4ucEIeMBZ4ChaF1Jh2GquGRHGvZ5EBQkVmWB%2BCikasNEfvRbqMcsqysWYTCuaQ1P8U9mbqeJklg6ZQU6dDTBAXsbkeUuDPIahI3oWzOGc8RPMsXzAsWELIuYGLdFMKOktMTLXHy0cESkKY7ZsTxSsCYJKusJgataU1MnCbiquierUnmTrQF6xwhvweHuLBqG4aRknWklRcidiTHaJx09edhl9hfl5JJcBmXyfa/uQERApdM5zXBfqZpjel5H0eSbHOvx73df98XQ9GkXo/SWnHDIF3/ELj3Ia14qCi53gNKg/xc5a4v%2B7cXB6%2BLn9AOfYqHrn7G7nXF4KSEbb/YyYyzJikcdogP9KZR12iaL4wRMrsmCBRQQAdIJ3jvIpecRtFwbnfHuYc7sxwTinECN%2BFwD5h0XNbbui4TZMzNo5DCWErZfS5vxW2EAJbjlYFgo4OCAwzgdm/HUwtiQuzsmRfoIAJZS2YrLCIqBVBAloHgcYtovRnAuOQFyQduI0VDhfJcE9N593njGJu2BJ7bxHro2eSpU5STYgRc4wcDiWHUUJFSd4sLVy0VPUSA9jHN2HmYrioDXJZ1CiZMyuk9gbANufRBaNr7iTXlCWMmd9CcFiLwPwHAtCkFQJwNwMVcLCNWDYjYPBcyaEzgsEkEgNAaASRwSQvAWBxEqSktJGSOC8FRJUggxSFhwFgDARAKBUDJjoNEcglAKQJCGTEWoLBgBcA2GYSpWBnhM0wAANWytiAA8oqFJhSaAZWiKiCAERimkAiMEWoYJOCFLOcwYgYINkRG0G8K5vAKRsEEBshgtBLmpN4FgHkwA3BiBRi80gWB2RGHEL8sFeAkqVFeKiaFPYKgljWIU0BrQTlSIiFiO5HgsAnLkXU7g8SqAGGAAoNZmBNnbNBfwQQIgxDsCkDIQQigVDqGhboZoBgjAoGyfoPAERUSQAWD1doiKSpalrMABgXhslmAiLwVArxiCnCwCK9h5RKjOAgK4EYTRSCBCmEUEo2RkipAEAa81uQ0g9FNf0MYrQ3hVAmNap1bRXVdHtflPQDNhieEaH6iYPqZgxC4AsXJzKFi0ESckk5zS9jXAAEoAFk9gAHE3BvlmbiMwuIfwQFwIQNamwI28A6b8mCpAFpMCwDEdhZTYiVNjdU2p9TSCNOVZwVpIB2mdNIN0vpSxkQlhGRAMZEzQiR04KoAAHDdEqN1JDvmQMgPYubCmYHwO8bceh6XCFEOIFlB72VqBOdy0g2IsQJBeTGuNnaE2cA2SWBIJY9ioCoEmtNmbs0bo2HmgtWEPCDPoO/H45ailVoWLW%2BtlBSkdtbTUx90Lmm9v7dBqpZh23NpQ00ntUGtDVtbRseNqGCOVqIwsVVKRnCSCAA%3D%3D)

the parser is:
```
constexpr auto ident = lexeme(letter >> *(letter | d10 | char_<'_'>));
constexpr auto type_p = ident++ >> -(omit(char_<'<'>) >> ascip::req<1>([](auto&r){r.reset(new type());return r.get();}) % ',' >> omit(char_<'>'>));
```
NOTE: lambda for create reqursion holder has to return pointer (or smart pointer).

let's see the `type_p` parser closely
```
constexpr auto type_p =
     ident++ // ident is a ident parser, ++ is a request for next field in structure on next sequence element.
             // so the ident will be stored on first result field and next item to second
  >> -(      // - is an optional parser
       omit(char_<'<'>) // omits a value
    >> ascip::req<1> // reqursively calls parser. 1 - the number of sequence parsers (as current - number)
       ([](...){...}) // lambda for create object for store reqursion. it get an empty unqie_ptr what emplace_back to result.
       % ','
    >> omit(char_<'>'>)
  )
;
```
what is the `<1>`? let's rewrite the `type_p` parser like this:
```
constexpr auto constexpr auto subtype = omit(char_<'<'>) >> ascip::req<1>([](auto&r){r.reset(new type());return r.get();}) % ',' >> omit(char_<'>'>);
constexpr auto type_p = ident++ >> -subtype;
```
as we can see the `type_p` parser contains two sequences: 
1. `ident` and
2. optional `subtype` parser definition.

the `req` parser calls parser recursively by number starts from current. so `req<0>` calls the `subtype` parser and `req<1>` calls the `type_p` parser. (the numeration starts from zero.)

## inheritance
due to an language limitations we cannot parse into struct with inheritance same way as simple struct. [here is example](https://godbolt.org/#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:1,endLineNumber:2,positionColumn:1,positionLineNumber:2,selectionStartColumn:1,selectionStartLineNumber:2,startColumn:1,startLineNumber:2),source:'%23include+%3Ctuple%3E%0A%0A%23include+%3Chttps://raw.githack.com/zerhud/ascip/master/build/ascip.hpp%3E%0A%0A%0Astruct+base+%7B+char+a,+b%3B+%7D%3B%0Astruct+child+:+base+%7B+char+c%3B+%7D%3B%0A%0Atemplate%3Ctypename+gh,+template%3Cauto%3Eclass+th%3Dgh::template+term%3E%0Aconstexpr+void+example()+%7B%0A++++//+for+parse+in+base+class+due+language+limits+we+can+use:%0A++++//+++-+cast+method,+it+will+case+the+result+to+base+class%0A++++//+++-+()+operator+and+return+reference+or+pointer+to+what+we+want%0A++++constexpr+auto+parser+%3D+cast%3Cbase%3E(th%3C!'a!'%3E::char_%2B%2B+%3E%3E+th%3C!'b!'%3E::char_)+%3E%3E+th%3C!'c!'%3E::char_(%5B%5D(auto%26r)-%3Echar%26%7Breturn+r.c%3B%7D)%3B%0A++++static_assert(+(%7B+child+r%3B%0A++++++++parse(parser,+%2Bgh::space,+gh::make_source(%22a+b+c%22),+r)%3B%0A++++++++(r.a%3D%3D!'a!')+%2B+(2*(r.b%3D%3D!'b!'))+%2B+(4*(r.c%3D%3D!'c!'))%3B%0A++++%7D)+%3D%3D+7+)%3B%0A%7D%0A%0Ausing+parser+%3D+ascip%3Cstd::tuple%3E%3B%0A%0Aint+main(int,char**)+%7B%0A++++example%3Cparser%3E()%3B%0A++++return+0%3B%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:100,l:'4',m:50,n:'0',o:'',s:0,t:'0'),(g:!((g:!((h:compiler,i:(compiler:armg1320,deviceViewOpen:'1',filters:(b:'0',binary:'1',binaryObject:'1',commentOnly:'0',debugCalls:'1',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'-std%3Dgnu%2B%2B2b',overrides:!(),selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1),l:'5',n:'0',o:'+ARM+GCC+13.2.0+(Editor+%231)',t:'0')),header:(),k:50,l:'4',m:50,n:'0',o:'',s:0,t:'0'),(g:!((h:output,i:(compilerName:'x86-64+gcc+13.1',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+ARM+GCC+13.2.0+(Compiler+%231)',t:'0')),header:(),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',m:50,n:'0',o:'',t:'0')),l:'3',n:'0',o:'',t:'0')),version:4) showing how to parse in such case.

primary code is
```
struct base { char a, b; };
struct child : base { char c; };
constexpr auto parser = cast<base>(char_<'a'>++ >> char_<'b'>) >> char_<'c'>([](auto&r)->char&{return r.c;});
```

please note:
- `cast` parser `static_cast`s result into `base` type, so inner parser can work
- `()` operator is a special semact. the semact allows to transform the result (it's single argument). it is another method for parse with inheritance.

## left reqursion
we can also use `lreq` and `lrreq` parsers for left reqursion. for example let's parse plus and minus expression. [here is full example](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGe1wAyeAyYAHI%2BAEaYxBIapAAOqAqETgwe3r56icmOAkEh4SxRMVxxdpgOqUIETMQE6T5%2BXLaY9rkM1bUE%2BWGR0bG2NXUNmc0KQ93BvUX9pQCUtqhexMjsHOYAzMHI3lgA1CYbbgRe8fSH2CYaAIKb27uYB0dsLCQAnhdXt2ZbDDte%2B0ObnGxGCwE%2BNzufweTzcADdanhDAQId9fv9AUcFCDMKxUVCMY8gU4cXiNpdIZCfvcAUSjggCAR4goQAB6VnEJgAdwAdMBCAgmMgANY8tAsVkAL2iCABrKYCmQeHirJYCoI0VZES8dHQ8sVyp5CHi8XxNy8ySMe3itSUxAA%2BlyBUsCPaNeMFE8ACJ7BVK01YgjoEAgE5nTCfDZWc2W4B7DXEFjBMTevbjYMgBGg5HEwSDDMgsGR6O3G4grwOPaYVTxYiHEvlysRZPEN726u1g4AdhLVwAnOmQ14GHgAI5eTD2%2BIEOtHDuz7B7ehUFFRr79wcgYdjidTmdA%2BcXPag4AM%2Bvrrte89lmcVgh7VDxaJMIgOs4WvYgPbN5it9s14huwsTckhnXEWHMAA2a1QUECAQNJCCzGg1A5j2NAGHGc9L2va5G3vR9n1fe0kwYD8vx/Wo23nICELAvFkJg4ICHgoMQ1A4hwKgh80IwrC1xwtcb2IO8HyfTliJYLxaEcT9vxbaiANotiQA4rjGNrZjWIzNSGJQ3iBH4qxBIbW9K0IiSSHtfA4TwdA5Mov8aJMHs6M4vSmLgtz1P09DDNXYyryEvCzII8SXysxIuWiByFP/TsXOAlTdKQ6DNK85LEO41C/MwgKXKC3thNEmiv03LMkUEIELIit9vAUUgxKIqzSItRqask6THHa8LiJsuyeuat9UGish42iUixE%2BHsCtwr52SXTACDADhPXS%2B8CAQR5OIULqwSrADMGxVIvg1FgzhfCMjgIN4n2YNh41uq7Fww1Zp08moInoVNOPHPAdu09issY1BGr4januyiJtDmRKH1hIEH2h%2Btu0K9cbjOi6NSBTGDGx66nvuzAeRJi4dgVT0KuRRr1qYL7iZJx6nwUVFvI8jqSCBIFAdU4GULB/y9ipqqCeZkmeU%2BRi4VhmbKQHFTbJyCATAAVisZDUFVr0IHB30vCIbjpcS1AEaOIXsK9RqjeC/tOJOYgGAfOacJvHS%2BaayzZzcbm2dSh8Bbyg7a24zA0MS9H5YzRXCGVtWoM1lXtaYfXNalmWir7fs8CoXLxhoiBfp1HbErhHl1ogVCLbQ0vy8rm2NEzhvWiUeGubNuE5ob/sCsa0PO77O3lkdzWbZdkK3fov2Oa9n3Msn7KA/GNMVO3cdJ2nL3D3JEOw57A45eziBQ%2BPJah9bo5EYAKiujPB4d8/vbN8wzDI2haGf52r1doH55B3rOYvkcHmKUF65zCkNKc9VuLxF3rfU%2B98TZtzcHsFaEAVqm2QfEHky57xIIOGYMw%2BCLD4MIXgrBJ4GQYJQRwOYK1P5fF9tlf%2BM8gGMJBovcBnsSLBAtNA2BEc77Dyoag9BZDsGYBXFQ5%2BewAC0JCqHkLwKeXBgDkErVoesUeX9x4/3clPZhSDgHu1BmAj2tUSJ7T4UBAR8ChF4JERwBR4jJF4OkZfeRYiKEqMfmomhdCtEMLnnophQ1DFsP5qY6e1k8C2XQFY8OctBEP0Rg4pxOCpEEL2KyDxqjrQ8i8cIvxmjexj3CWY18YSgk%2BX9pE5hU4RrRHibLUsA9bHJLNqksR6TXGZMvu46RnilGUPsUU%2BhNwFpqjeN%2BR4bwliAVEI7LkoINTTMZNERq2p7yED2NoC095DAHVYOGT8sNv681/ihOp74WQgHLmUnKus4ZJMQbky%2Bm08AsyjKjQJE9gl/wga1G5dyqnswMoHJ5bSXk%2BL2G8hAHyUazTOSA/5XCpIyQoLc2CLF7lgqXhC%2B2djXnvM%2BcQxFOjzl/MuRA/qGZgW/OqQ8wW%2BKz5QqvsShFpSQX6IgVFfomKtI4tMcyhBVDYXwq%2BWS06mBzp4yuscQmrBHinkarjS6QJk4G3JOTbE8YECHC9KeEMqqVkJggtvG4GqTZqmFJOYAnIWBqmIBAfhcsFpciYFMog6F3IrMLsQZIcJHjziOskAQexggn12jJCOC0SB7GiuhA5yAfWPBuk%2BeNApvW4ndBmzaewQhcnDY7BwcbDD2QUMKZUkauokSYDausctLXVpkrW%2BtqZVZWEThAS13FiDp2PPko6S0IAFr2DzLAog2yrjcBOmST0ICXz7RcdOBVYZfKScQPkw612kpMhHJtprUyGpAMxe03ZkHQCQCAMQ8RBRPEuOSGFl6Qw3sFC5Nwx70ClHfZtIEK17T%2BOwCGZAgoHRzB3TG7JtBfrhs9Okv1obHY2n9estMJttnjA9Q%2BR26SaCtDia6qDxAYO7KXshpQnoQiqHvMLAgjUE0hEwPZL1F1VjhoIo7Ta20hkokI0Q6wex%2BCAXGCQVNJslAYXsnh2g9kIBescopWsIYvGnJaUkiOexNOJv4m4BT8UFzPpANBzAo4C5HRrda6IYdt6Pt/UcOhlhAMhntCB2o96jzWGsMe6Dv0zNRtdJZvtqn%2Byaffdp6demt7YEM8Z0zO0LN1qs%2B52zer7McBkU5kALnQPJcXJ5yw3niMmb8wl%2Bt4GNPnvC0CSLAELgxd8/FltgXrMPsXHZtwK1L6Zey25yMeXLBeavT54rjWAuJaCxVsLogdM1eDuSerI3zNNfGy1o87WVqsm665wCfX%2BMFaG0VuLS2xtleCw3ULXZkHTYi3FKLC2jv%2BdbUl3bx7jWPFoIQLmBC%2Bkf0fbt/LFhCsNeO09ibcsLvIItJOWj9pRtydSx1jgaDNFAay9t3LexvNwhg7t9bozyTOe22dvsEPxqJkm/XPsC1aDY5M9aW0MUFnxhNlEE%2BrGmO5oQCfUc9OUOASdHmwUIoJZ8YYKgFZYgFAmyh/aGHo3ZFFvzSQNUtBtOPAgALhALpufLEQ9XRE1N6fYg5wqZtvGWkTL2czvYgpA1pkVVWegbBBB%2BWHPsz0XHed2gat%2BfWurHhUGHJUMNFblSemQMsTiLvghYFUL6Bg9kocsz457og8RFqBtVwibwjxTfy%2BCHnJg6ARe3DHl8cGNE4SoDsuNcY%2Bnu0pygtxBQKxGo9uQtxNARWHAupaTRUb/dNxoD9z0sw5H2A5OhS35AVDNyuHfpTptO2Ng%2BnH8rOW/ZLP2jtawR1QJx8Ok1y6N0R0CCfOiwsCO3dLAH8dM6fWJ%2BPQhgUDaVYpAr8N1v0fh/7oz8hi31LssKsKxC%2BHgMgC5uqECLOmmpgPBCsLDB3h3vNtPuBu/hvg3KNhBnLJuIWEYIhA%2BPrP3NrngsvtCugqIrkgPpTkPsQbkh/CrMcAMrkhXPrDyCCM6t6Pqn5N3gQNZmQesAwctI4ngi6GwTOBwaPqrG4AwFIWBP5iALIUwdCl3pxJWHgnPgngviUtol8MxHsGqMEBAMxKQNtn0r3v2NTktCtJ6EoI8FroWgmuPhHL/vXs/FwEQnsGYM/I1M/CrGOu4QJmYAgUEf3C4fOMrAQgEZYJ4QrhsN4SQiThAFEcQhAIQnIhsOVgQlgRbtktPDCjbqbvQDqppCQIQB8HLGEQBBEWYMkTEe4hsB4ZIPEW4Q0VALUakfkRkWHNEZIMEdkRYdkmWv7vbg9JrvoTwsni0pUbWNUbUWkXsA0XIk0QQj4ZEa0e0R0ekWhEsZkSEZTtMU6m4R4fMQ0QJssWYKsTUa0UkR4ZsQseBo0X0aEafq4QQjcYEWhPUY0c0ZEX4VAO8dEUEZ0d0cQr0c/P0Q3AcdUQCcQkCe4hAKcT0cEZcVwF2GOjCZ4Z8WOoiaCbsRCWpm0hoJ/DQvoJwCrLwH4BwFoKQKgJwN7ANtEYASsESD8DwKQAQJoDQgsMKLEHELQJwJILwCwCACrHEJSdSbSRwLwCyHEByVSSSXALADAIgCgKgOdHQOshQDrGqfEBqTEMAFwBsBcVgLZKsAAGp4CYBcgADyd0nAbJNAMk0QLIEAEQnJpACm9pvACm1p0MFQcpbJ4ozuBA1pDAtAbw7pWA2owAbgYgtALI3AvAWAaoRg4g8ppA%2BAqhjggaCZ1J1YFQ%2Bsaw1JzErQ7pH2EQnIrYHgWA7pM4eAwpiZCwVABgwACgFpVptpjAXpMgggIgYg7AUgPZ8gSgag7pugzQBgRgKAAO%2BgeAEQLIkACwj47QCZMi6Y%2BqwAZEAOZgEQvAqAgaxGdkR08ACw5QwefgR8CeIwTQpAgQUwhQxQWQSQKQAg15T5OQqQPQD5swLQbQVQEwb5YwrQ/p/5XQX5fQJQgwXQgFUFdQ4FMwJQp5cyqwEgCw/JHA5JpA4pe5nAewqgAAHJBDIpBJIJjsgDPoaTyIQhALgIQHGpsFwHMLwHKVoKgVtEXv0M6qQDyaKaSRwIKaQMKbxdhTSZwNKSALKZyQsIqSqS6PEPrOQJQOKLqfQMQKEIqpwARURSRWRRRRsDyGyUxnRaCMGM0PwL2aIOIIOeZcOSoOoOmeOaQEskwPEF6WhWSRSe6ZKdafrPJQRDnFpcRaRcAORXsJRdRR4OqapfghsIxcxVJQsOxVgDEFxTxXyQKZ5emZKeJZJfKagehWYEKSKWKV5WJeyQlXxRsJlRKWVSxVyaQAeYhiAJIEAA%3D). please note:
1. we use `lreq` parser only on leftmost part of parser in the variant
2. we use `lrreq` parser in other parts of parser in the variant. it just parses the next parser from the variant (from a expression parser).
3. the `lvreq` parse calls nearest variant in stack recursively
4. the `use_variant_result` function is used for skip index in resulting variant. if it won't be called the resulting variant must to same sized as the parser.
5. we can parse plus and minus as single parser, so it will be all left reqursive, but it can be like in the example: the minus operator is less priority then the plus operator (the expression (1+(2-3)) has same result as ((1+2)-3)).
 
# roadmap
- test, writing few parsers
- alfa release
- implement same struct as the ascip, containg parsers for print a parser it self, as grammar documentation.
- implement good double parser
- beta release
- remove all std includes
