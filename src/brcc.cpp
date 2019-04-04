/*
Binary Resources for C++ Compiler (BRCC) version 1.0.0
Copyright 2019 Ryan P. Nicholl <exaeta@protonmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include <iostream>
#include <string>
#include <exception>
#include <map>
#include <cctype>
#include <utility>
#include <fstream>
#include <iterator>
#include <vector>
#include <sstream>
static std::string progname;

static void help()
{
  std::cerr << "Binary Resources for C++ Compiler (BRCC) version 1.0.0" << std::endl << std::endl;
  std::cerr << "Usage: " << progname << " output_basename HEADER_GUARD [symbol_prefix binary_file]..." << std::endl << std::endl;
  std::cerr << "Example: " <<std::endl << "    " <<  progname << " ./build/gen/binary_resources GAME_BINARY_RESOURCES_HH bin_vertex_shader ./src/vertex_shader.glsl \\" << std::endl <<
   "    bin_fragment_shader ./src/fragment_shader.glsl" <<std::endl;
  std::cerr << "  Creates files './build/gen/binary_resources.hpp' and './build/gen/binary_resources.cpp'" << std::endl;
}

static std::string nicify(std::string const & input)
{
  std::string output;
  
  for (auto const & x: input)
  {
    if (isalnum(x) || x == '_')
    {
      output += x;
      
    }
    else output += "_";
  }
  
  return output;
  
  
}

static std::string escape(std::string const & input)
{

  try
  
  {
    std::string output;
    
    if (!isalpha(input.at(0)) && input.at(0) != '_')
    {
      throw std::invalid_argument ("expected alpha or underscore");
    }
    output += input.at(0);
    for (size_t i = 1; i < input.size(); i ++)
    {
      char c = input.at(i);
      
      if (!isalnum(c) && c != '_')
      {
        throw std::invalid_argument ("expeced alunum or underscore");
      }
      
      output += c;
    }
    
    
    return output;
    
    
  }
  
  catch (std::out_of_range &)
  {
    throw std::invalid_argument("Unexpected end of string");
  }
}



int main(int argc, char **argv)
{
  std::map<std::string, std::vector<uint8_t>> str;
  if (argc >= 1) progname = argv[0];
  else progname = "<program>";
  if (argc <= 3) 
  {
    help();
    return 0;
  }
  
  std::string outputname = argv[1];
  std::string guardname = escape(argv[2]);
  
  for (int i = 3; i < argc; i ++)
  {    
    std::string symname = argv[i];
    
    i++;
    if (i >= argc) throw std::invalid_argument("expected symbolic pair");
    std::string fname = argv[i];
    
    try
    {
      symname = escape(symname);
    }
    catch (std::invalid_argument & er)
    {
      std::cerr << er.what() << std::endl;
      return -1;
    }
    
    std::ifstream file(fname, std::ios::binary);
    if (!file) 
    {
      std::cerr << "Error opening file for reading" << std::endl;
      return -3;
    }
    std::vector<uint8_t> content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    str[symname] = std::move(content);
    

    
  }
  
  std::string header;
  std::string body;
  
  
  {
    std::stringstream hout;
  
    hout << std::string("#ifndef ")  << nicify(guardname) << std::endl;
    hout << std::string("#define ") << nicify(guardname) << std::endl;
    hout << "#include <cstdint>" << std::endl << std::endl;
    
    for (auto const & x : str)
    { 
      hout << "extern std::uint8_t const " << x.first << "[" << x.second.size() << "];" << std::endl;
    }
    
    hout << std::endl;
    
    hout << "#endif" << std::endl;
    
    header = hout.str();
  }
    
  {
    std::stringstream bout;
    bout << header;
    
    for (auto const & x : str)
    {
      
      
      bout << "std::uint8_t const " << x.first << "[" << x.second.size() << "] = {" << std::endl << "  ";
      for (size_t y = 0; y < x.second.size(); y++)
      {
        bout << "0x";
        
        int ca = (x.second[y] & 0xF0) >> 4;
        int cb = (x.second[y] & 0x0F);
        
        auto c = [](char q) -> char
        {
          if (q <= 9) return '0' + q;
          if (q <= 0xF) return 'a' + (q-10);
          else abort();
        };
        
        bout << c(ca) << c(cb);
         
        if (y != x.second.size() - 1)
        {
          bout << ",";
          if (y % 16 == 15)
          {
            bout << std::endl << "  ";
          }
          else bout << " ";
        }
      }
      bout << std::endl << "};" << std::endl;

    }
    body = bout.str();
  }
  
  std::ofstream headerf (outputname + ".hpp", std::ios::out | std::ios::trunc);
  std::ofstream bodyf(outputname + ".cpp", std::ios::out | std::ios::trunc);
  if (!headerf || !bodyf)
  {
    std::cerr << "Error opening file for writing" << std::endl;
    return -2;
  }
  headerf << header;
  bodyf << body;
  
  return 0;
  
}
