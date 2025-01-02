# ascip
c++ ascii not poisonous parser. requires c++23. for examples see a section "examples" below.

the parser was created as struct template, so you can parametrize your method for create grammar by it,
instead of include ascip file and depend on it.
the ascip needs in tuple for inner use,
and you can parametrize the structure with any tuple that have get method in adl.
the second template parameter, factory, can be the void type if you don't want to run the test method.

below, I've tried using examples for quick start.
all of them leave in the examples directory in actual state.
also I've tried to use the godbolt, but sometimes it can fail, unfortunately.
## how to install
it is a header-only library: for start using it include `ascip.hpp`.
there is also `build/ascip.hpp` file, it is a whole library in single file.
the file is used in examples (the godbolt can't include other files from url).
the file is built by gcc, and cann't to be used with clang, so there is
`build/ascip_clang.hpp` file for use with clang.

after the `ascip.hpp` is included the parser is `using parser = ascip<>;`

in the root is `flake.nix` file. so you can use it as flake input and develop with `nix develop`.
more information about [nix](https://nixos.wiki/wiki/Main_Page) and [nix flakes](https://nixos.wiki/wiki/Flakes).

there is no `make install` target.
for install copy the `ascip.hpp` and `ascip` to directory where your compiler will find it,
or pass `-I$(path_to_ascip_dir)` to compiler.
## how to parse
there is three steps:
1. create a parser specialization
2. create a data source
3. use a parse method

in most cases each step is just a single code line.
### create a parser specialization
for most cases the parser creation is just a `using parser = ascip<>;`
(the template parameter can to be a factory, it used for tests and will be removed soon).
### create a data source
in most cases just we can create a data source for parser with `parser::make_source()` method. for example, if all data is in a std::string_view object named `view` we can create the data source as `parser::make_source(view)`.

there is a few requirements on source type:
- it has to be lightweight copiable. the `parser::make_source()` method can work with `std::string`, but the parse process will be very slowly.
- the `parser::make_source()` method just creates a wrapper for data. any type can to be used as data source if it
- - has `operator()` returning a next char (the next call of the operators returns a next symbol)
- - has `operator bool` true - there is a more input, false - there is no next character to parse
- - has `operator+=(int)` increments current position of the source object 
### use a parse method
there is a few parse methods (`src` parameter is a data source described above). all methods returns count of parsed symbols or `-1` if parse fails:
- `parse(parser, src)` try to parse `src` with a `parser` and returns count of parsed symbols
- `parse(parser, src, result)` do the same as above and try to store result into the `result` parameter
- `parse(parser, skip_parser, src, result)` do the same as above and skip all what corresponds to the `skip_parser`
- `parse(parser, skip_parser, src, result, error_handler)` to the same as above and call the `error_handler` on error in parse (see the `>` parser)
- `parse_with_ctx(ctx, parser, src, result)` tries to get the `skip_parser` and `error_handler` from context. the methods is for calling from external parser - a parser class created by user.
## parser list
here is a list of available parsers. you can find examples below
- `int_` an integer if it can to be stored to result
- `uint_` an unsigned integer if it can to be stored to result, also notation can to be provied as template parameter, default is 10
- `d10` or `digit` for parser 0-9
- `lower` and `upper` for parse ascii lower or upper letters. and `letter` is a `lower` or `upper`.
- `space` means spaces
- `any` parses any character (if the character is not an ascii store (use `push_back`) it in the result)
- `nl` parses new line character
- `quoted_string` parsers string in single or double-quoted with escaping character \\. also `dquoted_string` parses only string and double quotes and `squoted_string` - single.
- `char_<'a'>` char with concrete value (it can to be wide char and so on). and `_char<'a'>` is same with omitted value.
- `lit<"string">` is a string literal. please note the string literal right inside the template parameter. unfortunatly it can to be called only with template keyword, or, with same way as terms parsers, but using `sterm` insead of `term` or `tmpl`.
- `operator |` for parse variant. the result will be created with 1) `template<auto ind> create(auto& var)` method or with 2) `template<auto ind> constepxr auto& emplace(auto &var)` 3) `template<auto ind> constexpr auto& emplace()` method. or 4) the result will be used as is
- `operator !` for negate parser
- `unary -` for parse optional value. if there is no value, the default constructor will be used.
- `binary -` for parse one value except other
- `*` and `+` for lists. `*` - zero or more times, `+` - one or mote times.
- `%` for parse separated values
- `()` with lambda for the semantic action (semact) or for create the result. if the functor inside `()` receaves reference to the parser result and returns reference or pointer it's a result maker. in other case it's a semact. the semact can to receave nothing, or the returned value by parser and the result, or the returned value by parser, the parsing context, the source and the result.
- `as` method for treat some parser as value
- `omit` method for skip value
- `add_to_ctx` and `from_ctx` methods allows to store value in context and get it later in inner parser.
- `result_from_ctx` methods allows to use value stored in context with mehtod `add_to_ctx` as result, the original result will be passed as first parametor to the action.
- `>>` for sequence parser
- `>` for sequence parser. it causes an error if the parser fails with a message "unknown" (see must method).
- `check` method checks that the parser got as a result exactly required type
- `cast` method try to `static_cast` gotten a result to the required type. it is useful for parse to struct with inheritance as a result due to language limitations. see example below.
- `rv` method for parse reverse variant with left recursion. see example below. the result will be created same way as in the `|` operator.
- `reparse` method for parse and shift source position no zero, so the next parser will start from the same place.

with sequence parser can be used
- `cur_pos` just stores to result current position, parse nothing
- `cur_shift` for store to its result current shift position from sequence start
- `req<number>` for call the sequence (or parent sequence) recursively. the req parser also can be combined with `()` operator with lambda for create recursion result value.
- `++` prefix or postfix for increase result's field number to store the parser result. prefix `++` will increase the number for current parser and postfix `++` - for next parser.
- `--` same as `++` but decrease the number
- `finc<number>` method. same as `++` or `--` but you can specify the number will be added to current position (the number can also to be negative). the parser should to be the most outter one: char_<'a'> >> -finc<3>(char_<'b'>) will not work.
- `fnum<number>` method. setts the resulting field position independently of the current one. this parser as the `finc` should to be the most outer one.
- `use_seq_result` method. use result as it is inside the sequence parser instead of fields from that result.
- `must` method. causes an error on fail. accepts a message as template parameter, it will be passed to error function, passed in parse method. parameters: result, source on start parser position, current line number, message.
- lambda in sequence: sequence parser will call it with the same arguments as must method. its returned value, if present, will be added to the source position (or causes error if less then 0).

# examples

to parse something, we have to provide a data source. for ascip it's a class what has `operator+=`, `operator bool` and `operator()`.
- the `operator()` returns symbol and increments the position.
- the `operator+=` moves the position forward and
- the `operator bool` returns true if there is next symbol
- NOTE: the object has to be light wight: it will be copied many times

`ascip::make_source` can create such objects from string_view or string literal.

## simple parser

for example, we want to parse version number like `v1.2`. the major version is `1` and minor version is `2`. code to parse it may looks like
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
what about reqursion? we can write a class wrapper with redefined operator = for it can be used to create a value. or we can create reqursion value in lambda. for example let's parse a type like `box<list<string>, int>`. i use a vector of unique_ptr for simplify this example. [please see full example on godbolt](https://godbolt.org/z/7cs3dTq83)

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
due to an language limitations we cannot parse into struct with inheritance same way as simple struct. [here is example](https://godbolt.org/z/rb8Gsrcfz) showing how to parse in such case.

primary code is
```
struct base { char a, b; };
struct child : base { char c; };
constexpr auto parser = cast<base>(char_<'a'>++ >> char_<'b'>) >> char_<'c'>([](auto&r)->char&{return r.c;});
```

please note:
- `cast` parser `static_cast`s result into `base` type, so inner parser can work
- `()` operator is a special semact. the semact allows to transform the result (it's single argument). it is another method for parse with inheritance.
- instead of casting the result we can provide a static method `struct_fields_count()` in the type, returning the count of fields in the type

## left reqursion
we can also use `rv_lreq` and `rv_rreq` parsers for left reqursion. for example let's parse some expression. [here is full example](https://godbolt.org/z/cEbvMhM3e). the example seems to big, you can pay attention on make_grammar function only.

```
	return rv( [](auto& r){ return std::unique_ptr<expr>( new expr{std::move(r)} ); }
	  , cast<ternary_expr>(gh::rv_lreq >> th<'?'>::_char >> ++gh::rv_rreq(result_maker) >> th<':'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> th<'+'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> th<'-'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> th<'*'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> th<'/'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> th<'%'>::_char >> ++gh::rv_rreq(result_maker))
	  , cast<binary_expr>(gh::rv_lreq >> gh::template lit<"**"> >> ++gh::rv_rreq(result_maker))
	  , rv_result(th<'('>::_char >> th<0>::rv_req >> th<')'>::_char)
	  , term
	);
```

please note:
1. we use `rv` function for create reversive variant. the variant parses from the last item to the first one. also, it dosen't stop on succssed parser (is stops only if parse terminals). note also, the most prioirty parser is on top.
2. we use `rv_lreq` parser only on leftmost part of parser in the variant (before terminal)
3. we use `rv_rreq` parser in other parts of parser (after terminal) in the variant. it just parses the next parser from the variant (from a expression parser).
4. we use `rv_req` parser to run the whole `rv` parser recursively. they parser accepts template parameter - a number of parser, the current `rv` parser is 0, outter parser, if present, is 1 and so on.
5. the `rv_result` function is used for skip index in resulting variant. if it won't be called the resulting variant must to same sized as the parser.
6. any parser can have semantic action and result maker methods. here it's done as `gh::rv_rreq(result_maker)`. the result maker should accept single parameter, in our case it will be the `right` field in `binary_expr` structure and returns the `expr`. the `result_maker` needed because `std::unique_ptr` is used and parser don't know how to create the field. but the field can has some type which creates result it self (for example in constructor and destroy result in descructor). in such case the `result_maker` can to be omitted. the result maker returns pointer or reference to created result.
7. the first `rv` parameter is also result creator. it creates the result for `left` field only. the result will be moved inside the parser.
8. we can parse plus and minus as single parser, so it will be all left recursive, but it can be like in the example: the minus operator is less priority then the plus operator (the expression (1+(2-3)) has same result as ((1+2)-3)).
9. `rv` parses as `n*m` where `n` is symbols count and `m` is parsers count
 
# roadmap
- test, writing few parsers
- alfa release
- implement the same struct as the ascip, containg parsers for print a parser itself, as grammar documentation.
- implement good double parser
- beta release
- remove all std includes
