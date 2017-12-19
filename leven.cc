
#include <sstream>
#include <vector>
#include <nan.h>

namespace std
{
};
using namespace std;

#include <node.h>

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Number;
using namespace v8;

const char* ToCString(const Nan::Utf8String& value) {
    return *value ? *value : "<string conversion failed>";
}

/*
The MIT License (MIT)

Copyright (c) Sindre Sorhus <sindresorhus@gmail.com> (sindresorhus.com)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
size_t levenshtein_distance(const char *ac, size_t al,const char *bc, size_t bl) {

    char *a = (char*)ac;
    char *b = (char*)bc;
    char* swap;
    swap = a;

    int aLen = int(al);
	int bLen = int(bl);

    if (al > bl) {
		a = b;
        b = swap;
        aLen = int(bl);
        bLen = int(al);
	}

	if (aLen == 0) {
		return bLen;
	}

	if (bLen == 0) {
		return aLen;
    }
    
    while (aLen > 0 && (a[~-aLen] == b[~-bLen])) {
		aLen--;
		bLen--;
    }
    
    if (aLen == 0) {
		return bLen;
    }
    
    int start = 0;

    while (start < aLen && (a[start] == b[start])) {
		start++;
    }
    
    aLen -= start;
    bLen -= start;
    
    if (aLen == 0) {
		return bLen;
    }
    
    char bCharCode;
	int ret = -1;
	int tmp;
	int tmp2;
	int i = 0;
    int j = 0;
    
    std::vector<int> charCodeCache = vector<int>(start + aLen);
    std::vector<int> arr = vector<int>(start + aLen);
    
	while (i < aLen) {
		charCodeCache[start + i] = a[start + i];
		arr[i] = ++i;
	}

	while (j < bLen) {
		bCharCode = b[start + j];
		tmp = j++;
		ret = j;

		for (int i = 0; i < aLen; i++) {
			tmp2 = bCharCode == charCodeCache[start + i] ? tmp : tmp + 1;
			tmp = arr[i];
			ret = arr[i] = tmp > ret ? tmp2 > ret ? ret + 1 : tmp2 : tmp2 > tmp ? tmp + 1 : tmp2;
		}
	}

	return (size_t)ret;
}

void Method(const FunctionCallbackInfo<Value>& args) {

    Isolate* isolate = args.GetIsolate();

    if (args.Length() < 2) {
        // Throw an Error that is passed back to JavaScript
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Wrong number of arguments")));
        return;
      }
    
      // Check the argument types
      if (!args[0]->IsString() || !args[1]->IsString()) {
        isolate->ThrowException(Exception::TypeError(
            String::NewFromUtf8(isolate, "Wrong arguments")));
        return;
      }
      
      Nan::Utf8String str_v(args[0]);
      Nan::Utf8String str_v1(args[1]);

      const char* str1 = ToCString(str_v);
      const char* str2 = ToCString(str_v1);
      
      double l1 = str_v.length();
      double l2 =  str_v1.length();
      
      auto result = levenshtein_distance(str1,l1,str2,l2);
      Local<Number> num = Number::New(isolate, result);
      args.GetReturnValue().Set(num);
}


void init(Local<Object> exports)
{
    NODE_SET_METHOD(exports, "levenshtein_distance", Method);
}


NODE_MODULE(NODE_GYP_MODULE_NAME, init)
