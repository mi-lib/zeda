ZTK - ZEDA Tag & Key format
=================================================================
Copyright (C) Tomomichi Sugihara (Zhidao) since 2019

-----------------------------------------------------------------
## [What is ZTK?]

ZTK is a file format to describe data.

-----------------------------------------------------------------
## [Concept]

The design concept of ZTK is summarized as follows.

- Syntax is not for computers but for humans. Too strong rules of
  syntax should be ruled out.
 - Parenthesization, indentation, delimination and line feed
   provides better readability for humans, but should not constrain
   the way of description.
- Humans can understand sentences mainly by context supported by
  keywords, so do computers as well.

-----------------------------------------------------------------
## [Syntax]

The following is a simple example along with on ZTK format.

```
[tag1]
key1: val1 val2 val3
key2: val4
key3: val5 val6

[tag2]
key1: val7 val8 val9
key1: val10 val11 val12
key2: val13

[tag1]
key1: val14 val15 val6
```

As you do, humans can understand the above [tag1] is not in the
same level with [Syntax] even though both are bracketted as well.

All the following lines are parsed in the same way.

```
key1 val1 val2 val3
key1: val1 val2 val3
key1;val1,val2,val3
key1 (val1, val2, val3)
key1: { val1, val2, val3 }
key1 val1:val2:val3
```

Obviously, the whitespaces, colons, semicolons, commas, parentheses
and braces are all identified as delimiters. Even the following
lines are accepted.

```
key1 (val1 val2 val3}
key1 (val1 val2 val3
```

The following also works.

```
key1
 val1
 val2
 val3
```

Except, the following has a different semantic.

```
key1 "val1 val2 val3"
```

Single/double quotations concatenate several splitted words into
one string. Actually, the parse unit of ZTK format is not a word
but a string.

Brackets have a special role to make tags.

-----------------------------------------------------------------
## [How to parse]

(to be described.)
