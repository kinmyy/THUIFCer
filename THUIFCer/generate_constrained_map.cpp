#include "stdafx.h"  
#include "matlib.h"
  #pragma hdrstop
  
  #include "generate_constrained_map.h"
  //#include "imdilate.h"
  //#include "iptchecknargin.h"
  //#include "morphop.h"
  //#include "padarray.h"
  //#include "mkconstarray.h"
  //#include "numel.h"
  //#include "iptcheckinput.h"
  //#include "iptcheckstrs.h"
  //#include "iptnum2ordinal.h"
  //#include "cellfun.h"
  //#include "regexpi.h"
  //#include "bwpack.h"
  //#include "bwunpack.h"
  //#include "uint32.h"

using namespace std;
  
  
  
  Mm generate_constrained_map(Mm M, Mm nb_theta, Mm obj, Mm theta_max) {
    begin_scope
    M.setname("M"); nb_theta.setname("nb_theta"); obj.setname("obj"); theta_max.setname("theta_max"); 
    dMm(M1); dMm(n); dMm(p); dMm(w); dMm(nw); dMm(x); dMm(Y); dMm(X); dMm(t); dMm(t_generate_constrained_map_v0);  \
      dMm(theta); dMm(R); dMm(Xr); dMm(Yr); dMm(SE); 
    
    #line 1 "c:/matcom45/generate_constrained_map.m"
    call_stack_begin;
    #line 1 "c:/matcom45/generate_constrained_map.m"
    // nargin, nargout entry code
    double old_nargin=nargin_val; if (!nargin_set) nargin_val=4.0;
    nargin_set=0;
    double old_nargout=nargout_val; if (!nargout_set) nargout_val=1.0;
    nargout_set=0;
    
    // translated code
    
    
    #line 3 "c:/matcom45/generate_constrained_map.m"
    // generate_constrained_map - generate a constraint map from a 2D map.
    #line 4 "c:/matcom45/generate_constrained_map.m"
    //
    #line 5 "c:/matcom45/generate_constrained_map.m"
    //   M1 = generate_constrained_map(M,nb_theta,obj);
    #line 6 "c:/matcom45/generate_constrained_map.m"
    //
    #line 7 "c:/matcom45/generate_constrained_map.m"
    //   M is the 2D map (binary image, 0 for background, 1 for objects).
    #line 8 "c:/matcom45/generate_constrained_map.m"
    //   M1 is a 3D array, M1(:,:,k) is the 2D map for orientation
    #line 9 "c:/matcom45/generate_constrained_map.m"
    //       theta=(t-1)*2*pi/nb_theta.
    #line 10 "c:/matcom45/generate_constrained_map.m"
    //  obj is the size of the size of the rectangle object that moves
    #line 11 "c:/matcom45/generate_constrained_map.m"
    //   (eg. obj=[0.2,0.05]).
    #line 12 "c:/matcom45/generate_constrained_map.m"
    //
    #line 13 "c:/matcom45/generate_constrained_map.m"
    //   Copyright (c) 2004 Gabriel Peyr?
    
    #line 15 "c:/matcom45/generate_constrained_map.m"
_   if (istrue(nargin()<2.0)) {
      #line 16 "c:/matcom45/generate_constrained_map.m"
_     nb_theta = 10.0;
      #line 17 "c:/matcom45/generate_constrained_map.m"
    }
    
    #line 19 "c:/matcom45/generate_constrained_map.m"
_   if (istrue(nargin()<3.0)) {
      #line 20 "c:/matcom45/generate_constrained_map.m"
_     obj = (BR(0.2),0.05);
      #line 21 "c:/matcom45/generate_constrained_map.m"
    }
    
    #line 23 "c:/matcom45/generate_constrained_map.m"
_   if (istrue(nargin()<4.0)) {
      #line 24 "c:/matcom45/generate_constrained_map.m"
_     theta_max = 2.0*pi;
      #line 25 "c:/matcom45/generate_constrained_map.m"
    }
    
    #line 27 "c:/matcom45/generate_constrained_map.m"
_   /*[n,p] = */size(M,i_o,n,p);
    #line 28 "c:/matcom45/generate_constrained_map.m"
_   M1 = zeros(n,p,nb_theta);
    
    #line 30 "c:/matcom45/generate_constrained_map.m"
_   w = max(obj);
    #line 31 "c:/matcom45/generate_constrained_map.m"
_   nw = norm(obj)*n+4.0;
    #line 32 "c:/matcom45/generate_constrained_map.m"
_   nw = round((nw-1.0)/2.0)*2.0+1.0;
    
    #line 34 "c:/matcom45/generate_constrained_map.m"
_   x = (colon(-(nw-1.0)/2.0,1.0,(nw-1.0)/2.0))*1.0/(n-1.0);
    #line 35 "c:/matcom45/generate_constrained_map.m"
_   /*[Y,X] = */meshgrid(x,x,i_o,Y,X);
    #line 36 "c:/matcom45/generate_constrained_map.m"
_   t_generate_constrained_map_v0 = colon(1.0,1.0,nb_theta); int t_generate_constrained_map_i0;
    for (t_generate_constrained_map_i0=0;t_generate_constrained_map_i0<t_generate_constrained_map_v0.cols();t_generate_constrained_map_i0++) \
       {
      forelem(t,t_generate_constrained_map_v0,t_generate_constrained_map_i0);
      #line 37 "c:/matcom45/generate_constrained_map.m"
_     theta = theta_max*(t-1.0)/nb_theta;
      #line 38 "c:/matcom45/generate_constrained_map.m"
_     R = (BR(cos(theta)),sin(theta),semi,
      -sin(theta),cos(theta));
      #line 39 "c:/matcom45/generate_constrained_map.m"
_     Xr = X*cos(theta)+Y*sin(theta);
      #line 40 "c:/matcom45/generate_constrained_map.m"
_     Yr = -X*sin(theta)+Y*cos(theta);
      #line 41 "c:/matcom45/generate_constrained_map.m"
_     SE = times((abs(Xr)<obj(1.0)/2.0),(abs(Yr)<obj(2.0)/2.0));
      #line 42 "c:/matcom45/generate_constrained_map.m"
_     M1(c_p,c_p,t) = imdilate(M,SE,Mc());
      #line 43 "c:/matcom45/generate_constrained_map.m"
    }
    call_stack_end;
    
    // nargin, nargout exit code
    nargin_val=old_nargin; nargout_val=old_nargout;
    
    // function exit code
    M.setname(NULL); nb_theta.setname(NULL); obj.setname(NULL); theta_max.setname(NULL); 
    return M1;
    end_scope
  }
  
  
  Mm generate_constrained_map() {
    begin_scope
    double old_nargin=nargin_val; nargin_val=0.0; nargin_set=1;
    dMm(M); dMm(nb_theta); dMm(obj); dMm(theta_max); 
    dMm(ret1a);
    ret1a=generate_constrained_map(M, nb_theta, obj, theta_max);
    nargin_val=old_nargin;
    return ret1a;
    end_scope
  }
  
  Mm generate_constrained_map(Mm M) {
    begin_scope
    double old_nargin=nargin_val; nargin_val=1.0; nargin_set=1;
    dMm(nb_theta); dMm(obj); dMm(theta_max); 
    dMm(ret1a);
    ret1a=generate_constrained_map(M, nb_theta, obj, theta_max);
    nargin_val=old_nargin;
    return ret1a;
    end_scope
  }
  
  Mm generate_constrained_map(Mm M, Mm nb_theta) {
    begin_scope
    double old_nargin=nargin_val; nargin_val=2.0; nargin_set=1;
    dMm(obj); dMm(theta_max); 
    dMm(ret1a);
    ret1a=generate_constrained_map(M, nb_theta, obj, theta_max);
    nargin_val=old_nargin;
    return ret1a;
    end_scope
  }
  
  Mm generate_constrained_map(Mm M, Mm nb_theta, Mm obj) {
    begin_scope
    double old_nargin=nargin_val; nargin_val=3.0; nargin_set=1;
    dMm(theta_max); 
    dMm(ret1a);
    ret1a=generate_constrained_map(M, nb_theta, obj, theta_max);
    nargin_val=old_nargin;
    return ret1a;
    end_scope
  }
  
