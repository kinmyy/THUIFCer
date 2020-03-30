#include "stdafx.h"
  
#include "matlib.h"
  #pragma hdrstop
  
  #include "rescale.h"
using namespace std;
  
  Mm rescale(Mm x, Mm a, Mm b) {
    begin_scope
    x.setname("x"); a.setname("a"); b.setname("b"); 
    dMm(y); dMm(m); dMm(M); 
    
    #line 1 "c:/matcom45/rescale.m"
    call_stack_begin;
    #line 1 "c:/matcom45/rescale.m"
    // nargin, nargout entry code
    double old_nargin=nargin_val; if (!nargin_set) nargin_val=3.0;
    nargin_set=0;
    double old_nargout=nargout_val; if (!nargout_set) nargout_val=1.0;
    nargout_set=0;
    
    // translated code
    
    
    #line 3 "c:/matcom45/rescale.m"
    // rescale - rescale data in [a,b]
    #line 4 "c:/matcom45/rescale.m"
    //
    #line 5 "c:/matcom45/rescale.m"
    //   y = rescale(x,a,b);
    #line 6 "c:/matcom45/rescale.m"
    //
    #line 7 "c:/matcom45/rescale.m"
    //   Copyright (c) 2004 Gabriel Peyr?
    
    #line 9 "c:/matcom45/rescale.m"
_   if (istrue(nargin()<2.0)) {
      #line 10 "c:/matcom45/rescale.m"
_     a = 0.0;
      #line 11 "c:/matcom45/rescale.m"
    }
    #line 12 "c:/matcom45/rescale.m"
_   if (istrue(nargin()<3.0)) {
      #line 13 "c:/matcom45/rescale.m"
_     b = 1.0;
      #line 14 "c:/matcom45/rescale.m"
    }
    
    #line 16 "c:/matcom45/rescale.m"
_   m = min(x(c_p));
    #line 17 "c:/matcom45/rescale.m"
_   M = max(x(c_p));
    
    #line 19 "c:/matcom45/rescale.m"
_   if (istrue(M-m<eps)) {
      #line 20 "c:/matcom45/rescale.m"
_     y = x;
      #line 21 "c:/matcom45/rescale.m"
    } else {
      
      #line 22 "c:/matcom45/rescale.m"
_     y = (b-a)*(x-m)/(M-m)+a;
      #line 23 "c:/matcom45/rescale.m"
    }
    call_stack_end;
    
    // nargin, nargout exit code
    nargin_val=old_nargin; nargout_val=old_nargout;
    
    // function exit code
    x.setname(NULL); a.setname(NULL); b.setname(NULL); 
    return y;
    end_scope
  }
  
  
  Mm rescale() {
    begin_scope
    double old_nargin=nargin_val; nargin_val=0.0; nargin_set=1;
    dMm(x); dMm(a); dMm(b); 
    dMm(ret1a);
    ret1a=rescale(x, a, b);
    nargin_val=old_nargin;
    return ret1a;
    end_scope
  }
  
  Mm rescale(Mm x) {
    begin_scope
    double old_nargin=nargin_val; nargin_val=1.0; nargin_set=1;
    dMm(a); dMm(b); 
    dMm(ret1a);
    ret1a=rescale(x, a, b);
    nargin_val=old_nargin;
    return ret1a;
    end_scope
  }
  
  Mm rescale(Mm x, Mm a) {
    begin_scope
    double old_nargin=nargin_val; nargin_val=2.0; nargin_set=1;
    dMm(b); 
    dMm(ret1a);
    ret1a=rescale(x, a, b);
    nargin_val=old_nargin;
    return ret1a;
    end_scope
  }
  
