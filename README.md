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
- `operator |` for parse variant. the result will be created with 1) `template<auto ind> create(auto& var)` method or with 2) `template<auto ind> constexpr auto& emplace()` method. or 3) the result will be used as is
- `operator !` for negate parser
- `unary -` for parse optional value. if tere is no value the default constructor will be used.
- `binary -` for parse one value except other
- `*` and `+` for lists
- `%` for parse separated values
- `()` with lambda for semact or for create the result. if the functor inside `()` receaves reference to the parser result and returns reference or pointer it's a result maker. in other case it's a semact. the semact can to receave nothing, or the returned value by parser and the result, or the returned value by parser, the parsing context, the source and the result.
- `as` method for treat some parser as value
- `omit` method for skip value
- `cur_pos` just stores to result current position, parse nothing
- `>>` for sequence parser
- `>` for sequence parser. it causes an error if the parser fails with message "unknown" (see must method).
- `check` method checks that the parser got as result exactly required type
- `cast` method try to `static_cast` gotten result to required type. it usefull for parse to struct with inheritance as result due for language limitations. see example below.
- `rv` method for parse reverse variant with left reqursion. see example below. the result will be create same way as in the `|` operator.

with sequence parser can be used
- `cur_shift` for store to result current shift position from sequence start
- `req<number>` for call the sequence (or parent sequence) recursively. the req parser also can be combined with `()` operator with lambda for create recursion result value.
- `++` prefix or postfix for increate result's field number to store parser result. prefix `++` will increate the number for current parser and postfix `++` - for next parser.
- `--` same as `++` but decrease the number
- `finc<number>` method. same as `++` or `--` but you can specify the number will be added to current position (the number can also to be negative). the parser should to be the most outter one: char_<'a'> >> -finc<3>(char_<'b'>) will not work.
- `fnum<number>` method. setts the resulting field position independently the current one. this parser as the finc should to be the most outter one.
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
- [see full example on godbolt](https://godbolt.org/z/fn5qjbnxj)

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

[example in godbolt](https://godbolt.org/z/6anMfTGbs)

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
we can also use `rv_lreq` and `rv_rreq` parsers for left reqursion. for example let's parse some expression. [here is full example](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGIM6SuADJ4DJgAcj4ARpjEEgCsAMykAA6oCoRODB7evv6p6ZkCIWGRLDFxXEm2mPaOAkIETMQEOT5%2BAXaYDlmNzQQlEdGxCckKTS1teZ0TA6FD5SNViQCUtqhexMjsHOaJocjeWADUJoluBF4p9GfYJhoAgnsHR5in52wsJACet/dPZn2DEOXhOZzc42IoWAf0ez2Br3ebgAbs08IYCLCAUCQWDzgpIZhWFj4bi3uCnITiYk7nC4YCXqDyecEAQCCkFCAAPRc4hMADuADpgIQEExkABrQVoFhcgBesQQoK5TAUyDwKS5LFVBFiXKiXjo6BVao1goQKRSJMeXgyRmOKWaSmIAH1%2BaKNgQXbrxgp3gARY6q9VW/EEdAgECXa6YP6JKw2u3AY664gsUJiAPHcYRkCoqEYimCWzhyOQ6FxhNPR6QrwOY6YVQpYhnKu1%2BtRDPEb4uxvN04Adir9wAnDnI14GHgAI5eTAulIEFvnPvL7DHehUTHx/6j8cgSczucLpfg1e3Y5Q4Cs1u7gf%2B281pd1ggp2LMbu9pvEQfDjRj0sDynWd50XZc3HPGljjQBh0EfasANzQ8QJPcDIPXPAqG9Yg53gvdAOQ48wLPb8L0wl0qDEJQ8P/fd0iXIkWHMAA2B0oUECA6KpJizFY1AVmggRxjw%2B94PbV9UBSWImCIV1rltY4QGOTsPx7Vdfy4hjiV4tjQgITjAPo4hGJY45%2BMEhhhJ3USdyfHD60k6TZJddMGAUpSVOaNTvw0wzuNM5s9IM3MjJMnTzJgqyrBsttnwcqS%2BWclgvFoRxFOUrtvP7Ewh004ztNYwKOLysK%2BIEyLt2ih9bIecSzISmSSBdfBkTwdB0s8z91JyiwSoK3Tir8rSeLKiyopy6rYvsiSGucxslyYDrMq/bLcqG/KRoG/S%2Bs2iKhMqiaxLimanKatJ%2BViJbVJWn8ep2gL2O29bSrM8r9tvGLd0eHkUwQN5jIUFLXwIb4pMFY5CDADg/WS8YU1QZS3gUVgkbwBUgz9R1iGdP00EnV9QmOfN0WLBtVCOaEHSdWI/X5PlLUwdrCYIP7L2RF0AaBqnsdif46vUpT92JwtzlTa7V1IerTrk7wFElxzEqa1zbXl2alaBvBVel5q8Fa9AtcV115r5A3Grk1ALrIN80wzWg/iHQ6av%2BH76AIKHMce7NUDYBALd%2B/7MEB1LKdXQOMgEf5dRYa4ZNjUXQcYFGUwTi8YO2RctqYKJ6CzYzZzwAHgsjUL%2BtQSWKuTqTTNQKJtBWHqzKRcEzNr1tB0mr6HijmPdXBbuDF7%2BOpOYNhBTH25DlVP1hdJoqmmzzAx/BkGpIULF7vCtXwPBIuQBL3by/2om0RFi4E4UJe/h05F64dulEMjVqMn0kx4isXjUFf/0IArpgvCIUyN8eoI3BM3ZEH1/SSyAU7f8xlLjEAYGZESD4nwhX8pvaWoDzi733tXQ%2BllXznh0pgASPUvoPzzHgZ%2BEBX7v2Yp/eI38/4AOvrfP8I5RyYTGrqHyEA86GgBj1ZEgo54QH4hAgSwjRHiJgRw/8tQlCNywW4ImNE5EcPvJLEhai4GbEQZ/J2olUHF3QXxLeyicGmLMvguG%2B5CKgVPCuUiNJTIkN/Kce%2BmEIBuN0QgpR5xm4ACo47sN8fopu5xThmDMG5WgdtonIL5s9Uu5iAnYI3nxGxJ1DYLllgFUhd8EJhP8W4ZuUMIBQwiSolIgpNyvmUVEswUSLCNKqQ6QUV5WRtKhisKGiTjF7ysQrM2FiMnWO4VLHJys146RSAU0JmB4HhIaeUypDSal1LaeYJpABaVp6yOl4GvPUtJKiel9MMSg2qyTdqpNKekm5eCJnDKShrfJv5yHFJAac44qyOBtI2ZgLcWzonHECfsn5NTOknPuWcjgvTdiXKSWg4a1c7k7zGWXZ5W8dZ63eWQ%2B%2BXzukcAqf8g5myGnbOOFyCFsL2nQuJQi/p1yUUbTRZg05ljUXhSyZMs2C4La81mfMz5iy9ElLKSStZkLalAphc3KlgTwVUoOQylZ8KLnDiMSykx3KzEcthVytlPLsXa2NkwfFhTRxEvVaSgFsrgWUtBeYeItLm5QqOV09VTKkUDNwRgnJ8lOQgFEZit6BD3E2p%2BYElmVC26HT9UMnF0zIyhseeFcNcMG5RrpTGhAcb4zt2Rbq41%2Bqpka1TY9I1L09oRuzWKvx3zc2xrXoWhNOrBl6r5c5FqbVK1BTDRM%2BtSyJWRLzQWlp7bMU4vNf2warKa2ZvqUOS8DblnRpbfG7V07tbnRGCGqtg6K7DvFU2oJm623bvTaxMWXkbpzqegu0uS7I1rtHSowJMF2pOqaWcAAYm6sd5FnzMjpVSpSKro3kUorQail6rmR0wNHAeccz7DyTteSW/dY7gmYQY7Ak8CS/TOP6a8kZsO6mtkxFxjwKrqTw8cbUEp5zAD5CwbUxAIAivvj9fkTBvjw2gvlSj/Dsa6zeKHAkWQIaIM5qlchP0SDHAutBQwQmiSUZXm8d0LN1Ox1pqKY4YR%2BQyegrJIMsFswSg1KuoOXomO83vgxuT9mmDMduokQMtCv4QDw6ZYgbDLwdMDosiAxnji7ywKIHs243BRdSgnCAgSAu3DYRNeuhbinEGFKFjLk7PpOf/gjVMLAsxkZAHpF0g4VHQCQCAMQKQxTvDuFBQJtXIwNbFDlNw5X0BcA0N1lm4IoYugudgSMyAxSuhWHlhTNLiDs1oHnCGfpNkQE2agKgQYqO2wEqJ8OiCsbOjm2zDmy2qGXk9a%2BPhV2zJbcWiV3bq7ZxiYEBF7QtpXxHcDkZ%2Bax8CyCHrjxml1hrDHH4D%2BcYJA3hEGzN0AQ7UaC1HahAWHnUspxBANCoHCEfoszeDQbGr4FvcxRqmFb22XN6dklh1mVPYbAwRjEVT4ws451hy5ymSPaCI6U5s/bWRRUjoWxAU4b8fN%2BZ0illdxS7HASIo4iCzjsCi/C%2BeNauYvjIkwHw2%2BgY8tFvvscY45cdR93fHe9C7WseLeW3GdcQ3zgXL/WNyMLpJvNGaxeUHlhysLbO5gacfDA5Axcm52IpCXFQUd24KG4BEXjZAO7qbXuoI%2B4sH79mxA87B7s2H9zM3yHG9N1ZNw6Obq3Gt/7pbgfU8O4QMN3YlhXdJ49x5lr650%2BZ4D0Hlz%2BeI849HMXlnsXy9W%2B7zX6cdfiNO44DslvyfPf2%2BadYbv2fA%2B59Dw5gLg//zD9EKXsfyuq%2B29r8vmPUNAkL7b9PrvdX/fr97yH1K/ed9F5NyP8ER/myV4n3bqP9ejeXI1%2BKey%2Bd%2BkYD%2BOefe2%2BheRuH%2BB%2Bo%2By04%2B9%2Bp%2BU%2B5%2BDes%2BLqIBS%2BABK%2BvuKBPem%2BL%2BMBu%2BI4%2B%2BZu5w3%2Ba4J%2BLok%2B0%2B5WFGbwtAhAoC0SSq2y3uuB4BNuhB0B4eb%2BcBksD%2Bz%2B%2BkF%2BUqCebuN%2By%2Ba%2BZ%2BuBYhPqiei%2BKWghVG5Cs2wOxwto84M8XoLmIAJ2hMDAJA2otALObwEA32P4Omvs/8z2mwB2kiJ8s8U8jMGMtmXMDOgmYo2uJ2yMbADY9AbAgggk%2BMbh%2BO3MOMksBowMrMVAk4PQb2Cg1mHI0EmwxkwRoQWAqgFm7UWha8Gh4RRAKQG4mA2uphqI3gbwqo7hqUMm4wRI6Agod4CGtG%2B06kyIqAbUb44wN0vmRWLEpkCgWwkskuzEpkaA6%2BDg3GCE6kLmai%2B4eM8qkS2y32EGzqPyQxyAbS%2B4rgdssizmWY32NC98o4DmLorGrAHG4IlhboHo/83ogcBAraKuaw5Co41gNx1hnoDxvoZYjo2wpAbx/4nxdxXoPoTxkYZxCgGwWwOurOjgyA7uFBcW3QCWUknEWw9cvEAxNIGJyAM2gJJxsCIh6hCE%2B45YRg3EZk/8ainobSHmdKlS0qdKcxsiCxdJP6r8FwkGdKYi/8gokIXGAYJGgkkxBAkejJuw8QFwzJzcnoApS4QpnJ0pDAXJDEdm%2Bh0pmI6xdKExxk9YDSOxsEexWqLRDwdGPkDG4JN07u%2BkoxgxwxQY/R2JOkk23QEo0xo4sxIhaiBxIpRxQJI4ZxFx7GzQ1x1MroXx9x4JzxXGhJCE7xlgIJLM3xMZfx4omA8Z7xwJEZtxKZ0ZjxwaUJMJ2weJrxRJI4VOpJo4bpkoouVZOib6y4ppncekjGTAoQEAekpAbeSqnp/4Lsiy7scObwvsJmKm325C1pq4NC0SXA1gZg2yks2yiQEWXAzSxwZgWJW5ai0534s5Zg65YOi50Sy50SrqEAR5lgm5251Z/4e5zYB5V5LSuyxwiQS5jSZBl5G5EAr5qwt5aiimW8YKxwYoq2YcukJAhAvw98D5nG2y85lgZggS75lgkgH52yA4EW35YOv5IF/5G5kgAFsicFT5G5TS4Kq5YO6Fp5jSXAq5UAz5EWFFb5M2hFxF7CP0hg7U4Rfh2mhm0yU5jxvRCF5Fxweyq5eyNFAQdFDFTFUAf5AkUlhe0Sd5I4pFolx54lb5hFGFc5DFOF15eFElbF1FHFJ23FymhmkmB2pmWMjA%2BOSg%2BRCEGl0ShlL5AklFultFCFF5jF5FnlrF7F2yalrlZg7lN5IFEAVF15RFelh5WF/lx5gV0V7F5lsFwlM5mliQAGEVW5UVMVrqRF6UA2Pl0SZgrqR5OV2FTF%2BV4KqVLqxwRFAkSkpVO5MCuONKKmogiCVECMtZEoEMBMiCMoKQdAMOeAbA5C8JeAiJLhLQoucFtpB5eVgVMVLS0ly54ujCfRLCYxUuaWFZzmV23okkuSCkIpwAiy4IA2uJV1sWA4legS914I85uJAWhyxyhesi1qJ1RR51foXIIpN2xyp1KQANwpnmTVal6WkNgYVQxwd5RSb6t1pp8K%2BgnA8QvAfgHAWgpAqAnApSlgYO0Jmw2wUSiQPApABAmg8KawEoEgGgGgGNHAkgvALAIA8QzNONeNBNHAvAnIzNNNuN6NcAsAMAiAKA3sY19AZAFAP80t41cQwA9FAQWArU2wAAangJgPyAAPLDycBU00CpQ0yUBRC02kDo5G28Do5621zdDC1U0yhBEEB60MC0DfCW1YAGjABuBiCwY22kBYDahGDiAi3B0FyO3iacgR2NjdD/w7B416S1CW3MFRB8jdgeBYCW1LiTU21rBUAGDAAKDa260G2MBB38CCAiBiDsBSAyCCCKAqDqAR26BcD6CGDGDp76B4BRCciQBrCST1CWScA7I5gkbABuTp5mBRC8CoDa7Z5tSBzwBrBdAJF%2BDeKwRTB%2BAd3BDzBlAVB6BpDPxZA73H2FAj2DCH1LA1B1C9CzDn0d3r0j19AtDX3DCVAlj9BP3f3v0H2f0SBr0ln11rC0CY3Y2W183HCqAAAczEOyzEkgxwwAyAWx9FgoTSEAuAhASmewXAKwvAwtWgBJf0TAWAcQcZDNXNLNbNpAHNNDPN89nAAtIAQttNawYtktnoKQ/85AlAo1St4QKMnAcDCDSDKDaDxwGDVNjMuDUIEYHd1dwgog4gDdyjzdaglt7dpAdMTAKQBdLNWNpATD%2BNnAet/8vDEkW2YjiDyDqD6DiQmDEWHg0cStFNBDRDHDawZDFDlA9NnNzN4DrNkDEdfNrD7DItBJwTZg7NgTJjUDLD1N3jLNiQoTvNSTxDdNpAi9B2IAkgQAA%3D). the example seems to big, you can pay attention on make_grammar function only.

```
	return rv( [](auto& r){ return std::unique_ptr<expr>( new expr{std::move(r)} ); }
	  , cast<ternary_expr>(gh::rv_lreq >> th<'?'>::_char >> ++gh::rv_rreq(result_maker) >> th<':'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> th<'+'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> th<'-'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> th<'*'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> th<'/'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> th<'%'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> gh::template lit<"**"> >> ++gh::rv_rreq(result_maker))
	  , rv_result(th<'('>::_char >> gh::rv_req >> th<')'>::_char)
	  , term
	);
```

please note:
1. we use `rv` function for create reversive variant. the variant parses from the last item to the first one. also, it dosen't stop on succssed parser (is stops only if parse terminals).
2. we use `rv_lreq` parser only on leftmost part of parser in the variant (before terminal)
3. we use `rv_rreq` parser in other parts of parser (after terminal) in the variant. it just parses the next parser from the variant (from a expression parser).
4. the `rv_result` function is used for skip index in resulting variant. if it won't be called the resulting variant must to same sized as the parser.
5. any parser can have semantic action and result maker mthods. here it done as `gh::rv_rreq(result_maker)`. the result maker should accepts single parameter, in our case it will be the `right` field in `binary_expr` structure and returns the `expr`. the `result_maker` needed because `std::unique_ptr` is used and parser don't know how to create the field. but the field can has some type which creates result it self (for example in constructor and destroy result in descructor). in such case the `result_maker` can to be omitted. the result maker returns pointer or reference to created result.
6. the first `rv` parameter is also result creator. it creates the result for `left` field only. the result will be moved inside parser.
7. we can parse plus and minus as single parser, so it will be all left reqursive, but it can be like in the example: the minus operator is less priority then the plus operator (the expression (1+(2-3)) has same result as ((1+2)-3)).
8. `rv` parses as `n*m` where `n` is symbols count and `m` is parsers count
 
# roadmap
- test, writing few parsers
- alfa release
- implement same struct as the ascip, containg parsers for print a parser it self, as grammar documentation.
- implement good double parser
- beta release
- remove all std includes
