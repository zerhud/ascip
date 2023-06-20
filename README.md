# ascip
c++ ascii not poisonous parser

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
  int major;
  int minor;
};

using ascip::char_;
using ascip::int_;
using ascip::make_source;

static_assert( ({
  version result;
  parse(char_<'v'> >> int_ >> char_<'.'> >> int_, make_source("v1.2"), result);
  result.major + result.minor; }) == 3 );
```

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
what about reqursion? we can write a class wrapper with redefined operator = for it can be used to create a value. or we can create reqursion value in lambda. for example let's parse a type like `box<list<string>, int>`. i use a vector of unique_ptr for simplify this example. [please see full example on godbolt](https://godbolt.org/#z:OYLghAFBqd5QCxAYwPYBMCmBRdBLAF1QCcAaPECAMzwBtMA7AQwFtMQByARg9KtQYEAysib0QXACx8BBAKoBnTAAUAHpwAMvAFYTStJg1DIApACYAQuYukl9ZATwDKjdAGFUtAK4sGISQCcpK4AMngMmAByPgBGmMQgZgAcpAAOqAqETgwe3r7%2BQemZjgJhEdEscQnJtpj2JQxCBEzEBLk%2BfoG19dlNLQRlUbHxiSkKza3t%2BV3j/YMVVaMAlLaoXsTI7BzmAMzhyN5YANQmO27jxOHAp9gmGgCCu/uHmCdnAG6YDiQ3d49mewYBy8x1ObjYLBIAE9fg8/k8gS83m4EAQCKkFCAAPRY4hMADuADpgIQEExkABrQloFhYgBe8QQIKxTAUyDwqSxLFZBHiLLZHMJCFSqVhjweFy8DiOBChqVeJgA7FY4RoAuN0CAQBcrkdmGxTirxeqCJqQJ9vsQwRqtV4GHgAI5eTAAfVSBCtZ1l8t%2BO2wRwUXhiLu9mAUhvhioAIhHVeL7uECEdueEIInSMgycQAFTZpYnZV/I7Fo5eTJGI6s9mpLXoLgaWP3Eul8vASsCmsoLMuxvNsu6qscrX0NHxRtFktoBjjTCqVLESteIhHPBYQRvKNHeiqTBsCAj3kL33%2B7P7zCjo%2BKtxHOsaAvXzMtHtnMAcF2vm5LJbjtWzRzIF1WSUVoICOCAlQsANTS1HUK09CxUhaJQ0zXAhSHbastW5ClXQUNYNkwcCzDMVAKXMMwViOYhvx2I1m2bCArQAVgsBsmM3U4Yx2TdX1QV982sMCzCOU9mIsLgTHYzjONfCl%2BJoyClSjATuM4o4dnQ8jRAYI5EOIYCV1Q8iFPhB5mynGc5wXJgl1QGU5VdVINzA1dGAIaxBOPI4AFoIFQFhCAgR9iGfNwPxfbY/RU24/QwocQGITAHTBMwbnAljJKjCAbKIcwADZqO8m5Q3yiDiEJRKlAICAInxez5QgL9DUSgh1h07N4KUgSzCYo5X1IV83hi/1/MC4LQvC7BJq/EzVRNJh/0AhRgOqsCINDKjDT05DQzddDvMHTtRuq6xDpgxDNkos6QGw3D8M2IizCYYz0Oo5rCX1TBJNYzKI2jFSuJ4jgmEG2bjT/PAAKA%2BJVvA5UNvg7bCN21J9uu46iIsa6FAuzAro7LCmBwl08PWB7yOes4YgzX5iMot7aNMgI7gCRjCUDYNQ3DFi2KjIq/U%2B77eY3NTXxifiTksISRLA8qOZDBzufEzL%2BewQWed%2B1TuNfZB5L%2B5SRe49SjjBidiwhqHlph0C4YsBGtqQ5GHL2nz0YCk7LGx3H8cwm6ibusnCPImJUFUMFaDwcZrQ9K4bnIQRaYo17TbVFm2flrmhZVm4M8VrP2NVz7DY44jYOuOnJcgiBhOzWX2aDBX5SViSC5uQWtZjYjE2Mkz/uL42U6bEsWrao4G0ZuFow4FZaE4JjeD8DgtFIVBODcDypdJgjJZ2HhSAITRp5WCkQCYjR9E4SQF8PlfOF4TFz4Ppfp9IOBYBgRAUH81I6HichKBpD/egCRgBcF3qQLA7xIaYAAGp4EwPiAA8vKRee8aC0EPJiCAMQb4xHCC0KEnA954OYMQKEiCYjaC%2BE/PeNI2CCEQQwWghDn4QMwDELwwA3BiFoJibgvAsDciMOIVh%2BBEoODwJ8Phy9ZxfCXFsPeiY6g3wjjEPEZCPBYBvjHFgRCX5UAMMABQcCEHIMYHomQggRBiHYFISx8glBqBvroLg%2BhDDGA3jYVRmJIArFQO6bIfDvIak4qYSw1gAS8FQJ8YglwsA%2BMat0ah2QXAMHcJ4DoehQjhCGJUEYriihZAEFMPwBSMhFIYPMYYCRXF2GSQIPokwMn5FqXUepjQJgDByQsfJthOklL0LMVoVS8k1JWFvTYEgZ5z2vqw1eHAjiqCSHlbyeVJBHGAMgZARwwGEi4GBXAhASA7y4EsXgT8tBflICfM%2BF8OBX1ILo25i9l7zPviAR%2Bh8Vhv0/msdES5/4QEwPgIgcS9D8CsaIcQdiIUOJUOoVhLjSD4jxDWfh0yODz1IC8qJnBEFLlSEuI4qAqCLOWas9Zmztm7P2RADwLAgHxBOWc/eXyVgIEwEwLACREmz3ubwJ558cW3w4O8z5z8rk3PPnynYszXl31ZRKjFZg5W4tFYqy5KwYmZGcJIIAA%3D%3D%3D)

the parser is:
```
constexpr auto ident = lexeme(letter >> *(letter | d10 | char_<'_'>));
constexpr auto type_p = (ident++ >> -(omit(char_<'<'>) >> ascip::req<2>([](auto&r)->type&{r.reset(new type());return *r;}) % ',' >> omit(char_<'>'>)));
```
NOTE: lambda for create reqursion holder has to return reference.
