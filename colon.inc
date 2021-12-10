%define pointer 0 

%macro colon 2
%2: 
    dq pointer 
    db %1

%define pointer %2
%endmacro