# Colorfulpp
This header will add color to your c++ console application 

Well first of all I'd like to say that this library was inspired by Colorful.Console you can check them out here:
https://github.com/tomakita/Colorful.Console

## Write string in any colour

```cpp

std::string a = "Hello world!";
COLORREF color = RGB(8,76,44);

colorful::Write(a,color,middle);

```

this function will write string a in color with specified above in a middle of the screen.2nd and 3rd arguments are optional.
This function also has WriteLine varient.

## Write string in multiple colors

```cpp
using namespace colorful;

ColorfulString* a = MakeColorfulString("Hello ",RGB(8,76,44)) + MakeColorfulString("world", RGB(25,25,25));

Write(a,middle);

```

this function will write Hello in specified color and World in specified color in the middle.
This function also has WriteLine varient.

## Set color of the text in which user will type

```cpp

colorful::SetGlobalTextColor(RGB(25,25,25));

colorful::ResetGlobalTextColor();

```

SetGlobalTextColor will set color of text user writes to any color you specify.
and ResetGlobalTextColor will set it back to gray.

## Write a title in any figlet style
```cpp

colorful::WriteTitle("Text","Big.flf",RGB(25,25,25),Middle,false);

```

This function will print first parameter in figlet font defined by second parameter it can be read from file like in example above but it can also be imported from a string if it is imported from string you'll have to change last parameter to true.3rd,4th and 5th arguments are optional.
