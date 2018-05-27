#ifndef __HW_DEBUG_H__
#define __HW_DEBUG_H__

#include <cstdlib>
#include <cstdio>
#include <cstring>


class dbg {
	private:
		dbg(){};
		~dbg(){};
	public:
		static void print(const std::string &s){
			qDebug("%s",s);
		};
		static void print(long n){
			qDebug("%l",n);
		};
		static void println(const std::string &s){
			qDebug("%s",s);
		};
		static void println(long n){
			qDebug("%l",n);
		};
		static void printf(const char *format, ...){
			va_list argptr;
			va_start(argptr, format);
			vfprintf(stderr, format, argptr);
			va_end(argptr);
		};
};

#endif
