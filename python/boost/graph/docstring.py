# Copyright 2005 The Trustees of Indiana University.

# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http:#www.boost.org/LICENSE_1_0.txt)

#  Authors: Douglas Gregor
#           Andrew Lumsdaine

columns = 72

def wrap(text, width):
    """
    A word-wrap function that preserves existing line breaks
    and most spaces in the text. Expects that existing line
    breaks are posix newlines (\n).
    From: http://aspn.activestate.com/ASPN/Cookbook/Python/Recipe/148061
    """
    return reduce(lambda line, word, width=width: '%s%s%s' %
                  (line,
                   ' \n'[(len(line)-line.rfind('\n')-1
                         + len(word.split('\n',1)[0]
                              ) >= width)],
                   word),
                  text.split(' ')
                 )

class function_documentor:
    def __init__(self, name):
        self._name = name
        self._parameters = list()
        self._result = None
        self._signatures = list()
        self._paragraphs = list()
        self._example = None
        self._see_also = list()
        self._cppdocs = name + ".html"
    def parameter(self, name, doc, default = None):
        """ 
        Declares a function parameter given its name, a documentation
        string, and (optionally) a string containing its default
        value.
        """
        self._parameters.append((name, doc.strip(), default))
        return self

    def result(self, type):
        """
        Declares the type that will be returned by the function.
        """
        self._result = type
        return self

    def paragraph(self, para):
        """
        Add a paragraph of descriptive text.
        """
        self._paragraphs.append(para.strip())
        return self

    def signature(self, parameters, result = None):
        """
        Add a signature for the function. parameters is a tuple
        containing the names of the parameters used in the signature
        and result is the result type. If no signatures are added, a
        signature will be generated from the parameters.
        """
        self._signatures.append((parameters, result))
        return self

    def example(self, example):
        """
        Add example code, written in Python of course.
        """
        self._example = example
        return self

    def see_also(self, other):
        """
        Add the name of something else that the might want to refer
        to.
        """
        self._see_also.append(other)
        return self

    def cpp_docs(self, html):
        """
        Set the file name for the C++ documentation of this routine
        within the online Boost Graph Library documentation. This
        should be a full file name, e.g.,
        "bellman_ford_shortest.html", but is only required to be
        specified when the default (name + ".html") is incorrect. Note
        that "html" may be None to indicate that no link to the C++
        documentation should be provided.
        """
        self._cppdocs = html
        return self

    def __signature_string(self, parameters, result):
        """
        Builds a string containing the signature of this function, e.g.,

          __signature_string(self, parameters, result) -> str
        """
        docstring = self._name + '('
        if parameters != list():
            param_start_column = len(self._name + '(')
            column = param_start_column
            max_column = columns - 2
            for (on_param, param) in enumerate(parameters):
                # Add ", " when needed
                if on_param > 0:
                    docstring += ', '
                    column += 2

                # If we're on the last parameter and there is a
                # result, we have stricter requirements on maximum
                # column.
                if on_param+1 == len(parameters) and result != None:
                    max_column = columns - len(') -> ') - len(result)

                # Build a string for the parameter signature
                paramstr = param
                default=filter(lambda p: p[0] == param, self._parameters)[0][2]
                if default != None:
                    paramstr += ' = ' + default

                # If the parameter won't fit on this list, and we're
                # not already at the beginning of a new line, put in a
                # newline.
                if column + len(paramstr) > max_column and \
                   column > param_start_column + 2:
                    docstring += str('\n').ljust(param_start_column + 1)
                    column = param_start_column
                    
                # Add the documentation for this parameter
                docstring += paramstr
                column += len(paramstr)
        docstring += ')'

        if result != None:
            docstring += ' -> ' + result

        return docstring

    def __parameters_string(self):
        """
        Builds and returns a docstring containing documentation for
        each of the parameters to the function.
        """
        if self._parameters == list():
            return ''

        docstring = """

Parameters:
"""
    
        # Compute maximum length of any parameter name
        maxlen = 0
        for param in self._parameters:
            maxlen = max(maxlen, len(param[0]))

        # Build documentation for parameters
        for (on_param, param) in enumerate(self._parameters):
            if on_param > 0:
                docstring += '\n'

            docstring += '  ' + param[0].ljust(maxlen + 2)
            doc = wrap(param[1], columns - maxlen - 4)
            padding = str('')
            for line in doc.split('\n'):
                docstring += padding + line + '\n'
                padding = str('').ljust(maxlen + 4)
        
        # Pull off the final '\n'
        return docstring[0:len(docstring)-1]

    def __str__(self):
        # Start with the signature
        if self._signatures == list():
            docstring=self.__signature_string([p[0] for p in self._parameters],
                                              self._result)
        else:
            docstring=''
            for sig in self._signatures:
                if sig != self._signatures[0]:
                    docstring += '\n'
                docstring += self.__signature_string(sig[0], sig[1])

        # Add the documentation paragraphs
        for para in self._paragraphs:
            docstring += '\n\n'
            docstring += wrap(para, columns)

        # Add the documentation of each parameter
        docstring += self.__parameters_string()
                        
        # Add the example
        if self._example != None:
            docstring += """

Example:""" + self._example

        # Add the "See also" section:
        if self._see_also != list():
            docstring += """

See also:"""
            for other in self._see_also:
                docstring += '\n  ' 
                docstring += other

        # Add the "Complete C++ documentation" section
        if self._cppdocs != None:
            docstring += """

Complete C++ documentation is available at:
  http://www.boost.org/libs/graph/doc/""" + self._cppdocs

        return docstring

                    
                
                


class docstring:
    """
    Produces documentation in Python docstring format for the Boost
    Graph Library's Python bindings using a simple textual format
    suitable for display on simple terminals.
    """

    def function(self, name):
        """Generates a function documentor object"""
        return function_documentor(name)

