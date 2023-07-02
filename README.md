# ascip
c++ ascii not poisonous parser. requires c++23.
## parser list
here is a list of avaible parsers. you can find examples below
- `int_` an integer if it can to be stored to result
- `d10` or `digit` for parser 0-9
- `lower` and `upper` for parse asci lower or upper letters. and `letter` is a `lower` or `upper`.
- `space` means spaces
- `any` parses any character
- `nl` parses new line character
- `quoted_string` parsers string in single or double quoted with escaping character \\. also `dquoted_string` parses only string and double quotes and `squoted_string` - single.
- `char_<'a'>` char with concreate value (it can to be wide char and so on). and `_char<'a'>` is same with omitted value.
- `operator |` for parse variant
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
- `check` method checks that the parser got as result exactly required type
- `cast` method try to `static_cast` gotten result to required type. it usefull for parse to struct with inheritance as result due for language limitations. see example below.

with sequence parser can be used
- `cur_shift` for store to result current shift position from sequence start
- `req<number>` for call the sequence (or parent sequence) recursively. the req parser also can be combined with `()` operator with lambda for create recursion result value.
- `++` prefix or postfix for increate result's field number to store parser result. prefix `++` will increate the number for current parser and postfix `++` - for next parser.
- `--` same as `++` but decrease the number
- `finc<number>` method. same as `++` or `--` but you can specify the number will be added to current position (the number can also to be negative).
- `must` method. WIP. causes an error on fail.

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
- [see full example on godbolt](https://godbolt.org/#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:90,endLineNumber:13,positionColumn:90,positionLineNumber:13,selectionStartColumn:90,selectionStartLineNumber:13,startColumn:90,startLineNumber:13),source:'%23include+%3Chttps://raw.githack.com/zerhud/ascip/master/ascip.hpp%3E%0A%0A%0Astruct+version+%7B%0A++int+major%3D0%3B%0A++int+minor%3D0%3B%0A%7D%3B%0A%0Ausing+ascip::_char%3B%0Ausing+ascip::int_%3B%0Ausing+ascip::make_source%3B%0A%0Aconstexpr+auto+ver+%3D+lexeme(_char%3C!'v!'%3E+%3E%3E+int_+%3E%3E+_char%3C!'.!'%3E+%3E%3E+%2B%2Bint_)%3B+//+other+variant%0A%0Aconstexpr+void+fnc_must_to_be_constexpr()+%7B%0A++static_assert(+(%7B%0A++++version+result%3B%0A++++parse(_char%3C!'v!'%3E+%3E%3E+int_+%3E%3E+_char%3C!'.!'%3E+%3E%3E+%2B%2Bint_,+ascip::make_source(%22v1.2%22),+result)%3B%0A++++result.major+%2B+result.minor%3B+%7D)+%3D%3D+3+)%3B%0A%7D%0A%0Aint+main(int,char**)+%7B%0A++return+0%3B%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:100,l:'4',m:50,n:'0',o:'',s:0,t:'0'),(g:!((g:!((h:compiler,i:(compiler:g131,deviceViewOpen:'1',filters:(b:'0',binary:'1',binaryObject:'1',commentOnly:'0',debugCalls:'1',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'-std%3Dc%2B%2B23',overrides:!(),selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1),l:'5',n:'0',o:'+x86-64+gcc+13.1+(Editor+%231)',t:'0')),header:(),k:50,l:'4',m:50,n:'0',o:'',s:0,t:'0'),(g:!((h:output,i:(compilerName:'x86-64+gcc+13.1',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x86-64+gcc+13.1+(Compiler+%231)',t:'0')),header:(),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',m:50,n:'0',o:'',t:'0')),l:'3',n:'0',o:'',t:'0')),version:4)

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

[example in godbolt](https://godbolt.org/#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:2,lang:c%2B%2B,selection:(endColumn:26,endLineNumber:16,positionColumn:26,positionLineNumber:16,selectionStartColumn:5,selectionStartLineNumber:9,startColumn:5,startLineNumber:9),source:'%23include+%3Ctuple%3E%0A%23include+%3Clist%3E%0A%23include+%3Cvector%3E%0A%23include+%3Chttps://raw.githack.com/zerhud/ascip/master/ascip.hpp%3E%0A%0Aint+main(int,char**)+%7B%0A++++using+ascip::char_%3B%0A++++using+ascip::make_source%3B%0A++++static_assert(+(%7B%0A++++++++std::vector%3Cchar%3E+r%3B%0A++++++++parse(%2Bchar_%3C!'a!'%3E,+make_source(%22aaa%22),+r)%3B%0A++++++++r.size()%3B+%7D)+%3D%3D+3+)%3B%0A%0A++++std::list%3Cchar%3E+r%3B%0A++++parse(%2Bchar_%3C!'a!'%3E,+make_source(%22aaa%22),+r)%3B%0A++++return+r.size()+!!%3D+3%3B%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%232',t:'0'),(h:compiler,i:(compiler:g131,deviceViewOpen:'1',filters:(b:'0',binary:'1',binaryObject:'1',commentOnly:'0',debugCalls:'1',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'-std%3Dc%2B%2B23',overrides:!(),selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:2),l:'5',n:'0',o:'+x86-64+gcc+13.1+(Editor+%232)',t:'0')),header:(),k:100,l:'4',m:50,n:'0',o:'',s:0,t:'0'),(g:!((h:output,i:(compilerName:'x86-64+gcc+13.1',editorid:2,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x86-64+gcc+13.1+(Compiler+%231)',t:'0')),header:(),l:'4',m:50,n:'0',o:'',s:0,t:'0')),l:'3',n:'0',o:'',t:'0')),version:4)

## reqursion
what about reqursion? we can write a class wrapper with redefined operator = for it can be used to create a value. or we can create reqursion value in lambda. for example let's parse a type like `box<list<string>, int>`. i use a vector of unique_ptr for simplify this example. [please see full example on godbolt](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGIAOwArKSuADJ4DJgAcj4ARpjEIGZmpAAOqAqETgwe3r4BwemZjgLhkTEs8YnJtpj2JQxCBEzEBLk%2BfkG19dlNLQRl0XEJSSkKza3t%2BV3j/YMVVaMAlLaoXsTI7BzmAMwRyN5YANQmO27jxBHAp9gmGgCCu/uHmCdnAG6YDiQ3d49mewYBy8x1ObjYLBIAE9fg8/k8gS83m4EAQCKkFCAAPRY4hMADuADpgIQEExkABrQloFhYgBeCQQIKxTAUyDwqSxLFZBASLLZHMJCFSqVhjweFy8DiOBChqVeJn8VjhGgAnON0CAQBcrkdmGxTsrxeqCJqQJ9vsQwRqtV4GHgAI5eTAAfVSBCtZ1l8t%2BO2wRwUXliLu9mAUhvh/gAIhGVeL7hECEduREIInSMgycQAFTZpYnJV/I7Fo5eTJGI6s9mpLXoLgaWP3Eul8vASsCmsoLMuxvNsu6qscrX0NEJRtFktoBjjTCqVLESteIhHPBYQRvKNHeiqTBsCAj3kL33%2B7P7zCjo/%2BNxHOsaAvXzMtHtnMAcF2vm5LJbjtWzRzIF1WSUVoICOCBFQsANTS1HUK09CxUhaJQ0zXAhSHbastW5ClXQUNYNkwcDklQClzDMFYjmIb8diNZtmwgK1AgsBtAk3U4Yx2TdX1QV982sMCzCOU9GIsLgTFY9j2NfCleOoyDFSjPjOPYo4dnQsjRAYI5EOIYCV1Qsi5PhB5mynGc5wXJgl1QGU5VdVINzA1dGAIax%2BOPI4AFoIFQFhCAgR9iGfNwPxfbY/SU24/QwocQGITAHTBMS/XApjxKjCArKIcwADYqIg4hCXipQCAgSJ8Vs%2BUIC/Q14oIdYtMK4Bz2qiNoz4sxAiOV9SFfN4ov9Xz/MC4LQuwMavyMlUTSYf9AIUYDSrAiDQ0ow0dOQ0M3XQzzB07IbSusPaYMQzYKOOkBsNw/DNiIswmEM9D8powr9UwcTmPStrFI3FTXyYPqpuNP88AAoCEiW8ClVW%2BCNsIrbUh2i6DqIiwLoUU7MHOjssKYHCXTw9ZbrIh6zliDNfmSCjnqNO5VTphjCUDYNQ3DJiWKjTybjej6Od%2Bzjuo4WJeJOSwBKEsDCuZkM7LZ0T0q5v0efZr7lM419kFk76lI4zcdiOIGJ2LEGwYWiHQKhiwYfWpD4bs7avORvzDssdHMexzDLrx66icIsjYlQVQwVoPBxmtD0rhuchBEp8insNtUGaloMZflOWOcV7BpdZ3mFe51gFTVtjklg64qdFyCIEE7NJaZlOc6YsTWMznmi7IxNDKM9r%2Bb1g2fybEs6oao4GxoyMow4FZaE4QJeD8DgtFIVBODcNyxcJgjRZ2HhSAITRJ5WCkQECDR9E4SQ5/3pfOF4TFT73hfJ9IOBYBgRAUF81I6ASchKBpL/6CJGAFwbepAsDvFBpgAAangTA%2BIADy8p547xoLQQ8mIICxCvrECILQoScB3jg5gxAoTwNiNoL4D8d40jYIIeBDBaD4MfmAzAsQvDADcGIWgmJuC8CwNyIw4hmH4Hig4PAnweGL1nF8JcWwd6JjqFfEOsQ8QkI8FgK%2BEcWAEKflQAwwAFAwLgYgxgOiZCCBEGIdgUhzHyCUGoK%2BuguD6EMMYNeNhlGYkgCsVA7psg8M8hqdiphLDWABLwVAnxiCXCwF46q3RKHZBcAwdwngOh6DCBEIYlQRjOKKFkAQUw/B5IyAUhg8xhiJGcXYRJAg%2BiTDSfkapdRamNAmAMLJCxcm2HaUUvQsxWgVJyVUlYG9NgSCnjPS%2BzDl4cCOKoAAHDlTyOVJBHGAMgZARwQGEi4GBXAhASBby4EsXgD8tBflIEfE%2BZ8OAX1INom589F6zNviAe%2B%2B8Vgv3fmsdES5f4QEwPgIgMS9D8AsaIcQNjwV2JUOoZhTjSD4jxDWXhkyOCz1IM8iJnB4FLlSEuI4qAqDzKWSstZGytk7L2RADwLAAEJGOac3enyVgIEwEwLAiR4nTzubwR5p9sXXw4G8j5j9LnXNPrynY0yXk3xZeK9FZhZU4pFQqi5KwomZGcJIIAA%3D)

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
due to an language limitations we cannot parse into struct with inheritance same way as simple struct. [here is example](https://godbolt.org/#g:!((g:!((g:!((h:codeEditor,i:(filename:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,selection:(endColumn:14,endLineNumber:10,positionColumn:14,positionLineNumber:10,selectionStartColumn:14,selectionStartLineNumber:10,startColumn:14,startLineNumber:10),source:'%23include+%3Chttps://raw.githack.com/zerhud/ascip/master/ascip.hpp%3E%0A%0A%0Astruct+base+%7B+char+a,+b%3B+%7D%3B%0Astruct+child+:+base+%7B+char+c%3B+%7D%3B%0A%0Aconstexpr+void+example()+%7B%0A++++using+ascip::char_%3B%0A++++using+ascip::space%3B%0A++++constexpr+auto+parser+%3D+cast%3Cbase%3E(char_%3C!'a!'%3E%2B%2B+%3E%3E+char_%3C!'b!'%3E)+%3E%3E+char_%3C!'c!'%3E(%5B%5D(auto%26r)-%3Echar%26%7Breturn+r.c%3B%7D)%3B%0A++++static_assert(+(%7B+child+r%3B%0A++++++++parse(parser,+%2Bspace,+ascip::make_source(%22a+b+c%22),+r)%3B%0A++++++++(r.a%3D%3D!'a!')+%2B+(2*(r.b%3D%3D!'b!'))+%2B+(4*(r.c%3D%3D!'c!'))%3B%0A++++%7D)+%3D%3D+7+)%3B%0A%7D%0A%0Aint+main(int,char**)+%7B%0A++++example()%3B%0A++++return+0%3B%0A%7D'),l:'5',n:'0',o:'C%2B%2B+source+%231',t:'0')),k:100,l:'4',m:50,n:'0',o:'',s:0,t:'0'),(g:!((g:!((h:compiler,i:(compiler:g131,deviceViewOpen:'1',filters:(b:'0',binary:'1',binaryObject:'1',commentOnly:'0',debugCalls:'1',demangle:'0',directives:'0',execute:'1',intel:'0',libraryCode:'0',trim:'1'),flagsViewOpen:'1',fontScale:14,fontUsePx:'0',j:1,lang:c%2B%2B,libs:!(),options:'-std%3Dc%2B%2B23',overrides:!(),selection:(endColumn:1,endLineNumber:1,positionColumn:1,positionLineNumber:1,selectionStartColumn:1,selectionStartLineNumber:1,startColumn:1,startLineNumber:1),source:1),l:'5',n:'0',o:'+x86-64+gcc+13.1+(Editor+%231)',t:'0')),header:(),k:50,l:'4',m:50,n:'0',o:'',s:0,t:'0'),(g:!((h:output,i:(compilerName:'x86-64+gcc+13.1',editorid:1,fontScale:14,fontUsePx:'0',j:1,wrap:'1'),l:'5',n:'0',o:'Output+of+x86-64+gcc+13.1+(Compiler+%231)',t:'0')),header:(),k:50,l:'4',n:'0',o:'',s:0,t:'0')),l:'2',m:50,n:'0',o:'',t:'0')),l:'3',n:'0',o:'',t:'0')),version:4) showing how to parse in such case.

primary code is
```
struct base { char a, b; };
struct child : base { char c; };
constexpr auto parser = cast<base>(char_<'a'>++ >> char_<'b'>) >> char_<'c'>([](auto&r)->char&{return r.c;});
```

please note:
- `cast` parser `static_cast`s result into `base` type, so inner parser can work
- `()` operator is a special semact. the semact allows to transform the result (it's single argument). it is another method for parse with inheritance.

# roadmap
- test, writing few parsers
- implement error handler
- alfa release
- implement double parser
- beta release
- remove all std includes
