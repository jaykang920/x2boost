# String concatenation macro to build a delimiter-seperated string
macro(string_append var str delimiter)
  if((NOT DEFINED ${var}) OR (${var} STREQUAL ""))
    set(${var} "${str}")
  else()
    set(${var} "${${var}}${delimiter}${str}")
  endif()
endmacro(string_append var str delimiter)

# String concatenation macro
macro(string_concat var str)
  string_append(var str "")
endmacro(string_concat var str)
