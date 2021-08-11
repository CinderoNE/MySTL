#pragma once
#ifndef _MY_EXCEPTDEF_H_
#define _MY_EXCEPTDEF_H_

#include<stdexcept>
#include<cassert>
namespace mystl {
#define ASSERT_EXPR(expr)\
	assert(expr)
} //end of namespace mystl

#endif // !_MY_EXCEPTDEF_H_
