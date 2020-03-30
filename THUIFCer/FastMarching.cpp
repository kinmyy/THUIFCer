#include "StdAfx.h"
#include "FastMarching.h"

FastMarching::FastMarching(void)
{
}

FastMarching::~FastMarching(void)
{
}

int FastMarching::minarray(double *A, int l)
{
	int i;
	int minind=0;
	for (i=0; i<l; i++) { if(A[i]<A[minind]){ minind=i; } }
	return minind;
}

double FastMarching::pow2(double val)
{
	return val*val;
}

void FastMarching::roots(double* Coeff, double* ans) {
	double a=Coeff[0];
	double b=Coeff[1];
	double c=Coeff[2];

	if(a!=0) {
		ans[0]= (-b - sqrt(pow2(b)-4.0*a*c)) / (2.0*a);
		ans[1]= (-b + sqrt(pow2(b)-4.0*a*c)) / (2.0*a);
	}
	else {
		ans[0]= (2.0*c)/(-b - sqrt(pow2(b)-4.0*a*c));
		ans[1]= (2.0*c)/(-b + sqrt(pow2(b)-4.0*a*c));
	}
}

int FastMarching::maxarray(double *A, int l) {
	int i;
	int maxind=0;
	for (i=0; i<l; i++) { if(A[i]>A[maxind]){ maxind=i; } }
	return maxind;
}

int FastMarching::mindex2(int x, int y, int sizx) { return x+y*sizx; }    

bool FastMarching::IsFinite(double x) { return (x <= doublemax && x >= -doublemax); }
bool FastMarching::IsInf(double x) { return (x >= doublemax); }

bool FastMarching::isntfrozen2d(int i, int j, int *dims, bool *Frozen)
{
	return (i>=0)&&(j>=0)&&(i<dims[0])&&(j<dims[1])&&(Frozen[i+j*dims[0]]==0);
}
bool FastMarching::isfrozen2d(int i, int j, int *dims, bool *Frozen)
{
	return (i>=0)&&(j>=0)&&(i<dims[0])&&(j<dims[1])&&(Frozen[i+j*dims[0]]==1);
}


int FastMarching::p2x(int x) /* 2^x */
{
	/*    return pow(2,x); */
	int y=1; 
	int p2x[16] ={1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
	while(x>15) { x=x-15; y=y*32768; }
	return y*p2x[x];
}

void FastMarching::show_list(double **listval, int *listprop) {
	int z, k;
	for(z=0;z<listprop[1]; z++) {
		for(k=0;k<p2x(z+1); k++) {
			if((z>0)&&(listval[z-1][(int)floor((double)(k/2))]>listval[z][k])) {
				printf("*%15.5f", listval[z][k]);
			}
			else {
				printf(" %15.5f", listval[z][k]);
			}
		}
		printf("\n");
	}
}

void FastMarching::initialize_list(double ** listval, int *listprop) {
	/* Loop variables */
	int i;
	/* Current Length, Orde and Current Max Length */
	listprop[0]=0; listprop[1]=1; listprop[2]=2;
	/* Make first orde storage of 2 values */
	listval[0]=(double*)malloc(2 * sizeof(double));
	/* Initialize on infinite */
	for(i=0;i<2;i++) { listval[0][i]=INF; }
}

void FastMarching::destroy_list(double ** listval, int *listprop) {
	/* Loop variables */
	int i, list_orde;
	/* Get list orde */
	list_orde=listprop[1];
	/* Free memory */
	for(i=0;i<list_orde;i++) { free(listval[i]); }
	free(listval);
	free(listprop);
}

void FastMarching::list_add(double ** listval, int *listprop, double val) {
	/* List parameters */
	int list_length, list_orde, list_lengthmax;
	/* Loop variable */
	int i, j;
	/* Temporary list location */
	int listp;
	/* Get list parameters */
	list_length=listprop[0]; list_orde=listprop[1]; list_lengthmax=listprop[2];
	/* If list is full expand list */
	if(list_length==list_lengthmax) {
		list_lengthmax=list_lengthmax*2;
		for (i=list_orde; i>0; i--) {
			listval[i]=listval[i-1];
			listval[i] = (double *)realloc(listval[i], p2x(i+1)*sizeof(double));
			for(j=p2x(i); j<p2x(i+1); j++) { listval[i][j]=INF;  }
		}
		listval[0]=(double *)malloc(2*sizeof(double));
		listval[0][0]=min(listval[1][0], listval[1][1]);
		listval[0][1]=INF;
		list_orde++;
	}
	/* Add a value to the list */
	listp=list_length;
	list_length++;
	listval[list_orde-1][listp]=val;
	/* Update the links minimum */
	for (i=list_orde-1; i>0; i--) {
		listp=(int)floor(((double)listp)/2);
		if(val<listval[i-1][listp]) { listval[i-1][listp]=val; } else { break; }
	}
	/* Set list parameters */
	listprop[0]=list_length; listprop[1]=list_orde; listprop[2]=list_lengthmax;
}

int FastMarching::list_minimum(double ** listval, int *listprop) {
	/* List parameters */
	int list_length, list_orde, list_lengthmax;
	/* Loop variable */
	int i;
	/* Temporary list location */
	int listp;
	/* Index of Minimum */
	int minindex;
	/* Get list parameters */
	list_length=listprop[0]; list_orde=listprop[1]; list_lengthmax=listprop[2];
	/* Follow the minimum through the binary tree */
	listp=0;
	for(i=0;i<(list_orde-1);i++) {
		if(listval[i][listp]<listval[i][listp+1]) { listp=listp*2; } else { listp=(listp+1)*2; }
	}
	i=list_orde-1;
	if(listval[i][listp]<listval[i][listp+1]){minindex=listp; } else { minindex=listp+1; }
	return minindex;
}
void FastMarching::list_remove(double ** listval, int *listprop, int index) {
	/* List parameters */
	int list_length, list_orde, list_lengthmax;
	/* Loop variable */
	int i;
	/* Temp index */
	int index2;
	double val;
	double valmin;
	/* Get list parameters */
	list_length=listprop[0];
	list_orde=listprop[1];
	list_lengthmax=listprop[2];
	/* Temporary store current value */
	val=listval[list_orde-1][index];
	valmin=INF;
	/* Replace value by infinite */
	listval[list_orde-1][index]=INF;
	/* Follow the binary tree to replace value by minimum values from 
	* the other values in the binary tree. 
	*/
	i=list_orde-1;
	while(true) {
		if((index%2)==0) { index2=index+1; } else { index2=index-1; }
		if(val<listval[i][index2]) {
			index=(int)floor(((double)index2)/2.0);
			if(listval[i][index2]<valmin) { valmin=listval[i][index2]; }
			listval[i-1][index]=valmin;
			i--; if(i==0) { break; }
		}
		else { break; }
	}
}

void FastMarching::list_remove_replace(double ** listval, int *listprop, int index) {
	/* List parameters  */
	int list_length, list_orde, list_lengthmax;
	/* Loop variable  */
	int i, listp;
	/* Temporary store value  */
	double val;
	int templ;
	/* Get list parameters */
	list_length=listprop[0];
	list_orde=listprop[1];
	list_lengthmax=listprop[2];
	/* Remove the value */
	list_remove(listval, listprop, index);
	/* Replace the removed value by the last in the list. (if it was
	* not already the last value)  */
	if(index<(list_length-1)) {
		/* Temporary store last value in the list  */
		val=listval[list_orde-1][list_length-1];
		/* Remove last value in the list  */
		list_remove(listval, listprop, list_length-1);
		/* Add a value to the list  */
		listp=index;
		listval[list_orde-1][index]=val;
		/* Update the links minimum  */
		for (i=(list_orde-1); i>0; i--) {
			listp=(int)floor(((double)listp)/2);
			if(val<listval[i-1][listp]) { listval[i-1][listp]=val; } else {  break; }
		}
	}
	/* List is now shorter */
	list_length--;
	/* Remove trunk of binary tree  / Free memory if list becomes shorter */
	if(list_orde>2&&IsInf(listval[0][1])) {
		list_orde--; 
		list_lengthmax=list_lengthmax/2;
		/* Remove trunk array */
		free(listval[0]);
		/* Move the other arrays one up */
		templ=2;
		for (i=0; i<list_orde; i++) {
			listval[i]=listval[i+1];
			/* Resize arrays to their new shorter size */
			listval[i] = (double *)realloc(listval[i], templ*sizeof(double));
			templ*=2;
		}
	}
	/* Set list parameters */
	listprop[0]=list_length; listprop[1]=list_orde; listprop[2]=list_lengthmax;
}

void FastMarching::listupdate(double **listval, int *listprop, int index, double val) {
	/* List parameters */
	int list_length, list_orde, list_lengthmax;
	/* loop variable */
	int i,listp;
	/* Get list parameters  */
	list_length=listprop[0];
	list_orde=listprop[1];
	list_lengthmax=listprop[2];
	/* Add a value to the list */
	listp=index;
	listval[list_orde-1][index]=val;
	/* Update the links minimum */
	for (i=(list_orde-1); i>0; i--) {
		listp=(int)floor(((double)listp)/2);
		if(val<listval[i-1][listp]) { listval[i-1][listp]=val; } else { break; }
	}
	/* Set list parameters */
	listprop[0]=list_length; listprop[1]=list_orde; listprop[2]=list_lengthmax;
}

/* The matlab mex function  */
mxArray* FastMarching::msfm2d( mxArray *SpeedImage, mxArray *SourcePoint,bool a, bool b ) {
	/* The input variables */
	double *F, *SourcePoints;
	bool *useseconda, *usecrossa;
	bool usesecond=true;
	bool usecross=true;
	mxArray *distanceMap;

	/* The output distance image */
	double *T;

	/* Current distance values */
	double Tt, Tt2;

	/* Derivatives */
	double Tm[4]={0, 0, 0, 0};
	double Tm2[4]={0, 0, 0, 0};
	double Coeff[3];

	/* local derivatives in distance image */
	double Txm1,Txm2,Txp1,Txp2;
	double Tym1,Tym2,Typ1,Typ2;
	double Tr1m1,Tr1m2,Tr1p1,Tr1p2;
	double Tr2m1,Tr2m2,Tr2p1,Tr2p2;

	/* Constant cross term */
	const double c1=0.5;

	/* Matrix containing the Frozen Pixels" */
	bool *Frozen;

	/* Size of input image */
	const int *dims_c;
	int dims[2];

	/* Size of  SourcePoints array */
	const int *dims_sp_c;
	int dims_sp[2];

	/* Return values root of polynomial */
	double ansroot[2]={0, 0};

	/* Number of pixels in image */
	int npixels;

	/* Derivative checks */
	bool ch1, ch2;

	/* Order derivatives in a certain direction */
	int Order[4]={0, 0, 0, 0};

	/* Neighbour list */
	int neg_free;
	int neg_pos;
	double *neg_listv;
	double *neg_listx;
	double *neg_listy;

	int *listprop;
	double **listval;

	/* Neighbours 4x2 */
	int ne[8]={-1, 0, 1, 0, 0, -1, 0, 1};
	/* Loop variables  */
	int z, k, itt, q, t;

	/* Current location */
	int x, y, i, j, in, jn;

	/* Index */
	int IJ_index, XY_index, index;


	/* Get the sizes of the input image */
	if(mxGetNumberOfDimensions(SpeedImage)==2)
	{
		dims_c = mxGetDimensions(SpeedImage);
		dims[0]=dims_c[0]; dims[1]=dims_c[1];
		npixels=dims[0]*dims[1];
	}
	else
	{
		mexErrMsgTxt("Speed image must be 2d.");        
	}

	/* Get the sizes of the  SourcePoints */
	dims_sp_c = mxGetDimensions(SourcePoint);

	if(dims_sp_c[0]!=2) {
		mexErrMsgTxt("SourcePoints must be a 2xn matrix.");
	}
	dims_sp[0]=dims_sp_c [0]; dims_sp[1]=dims_sp_c[1];


	/* Get pointers/data from  to each input. */
	F=(double*)mxGetPr(SpeedImage);
	SourcePoints=(double*)mxGetPr(SourcePoint);
	//if(nrhs>2){ useseconda = (bool*)mxGetPr(prhs[2]); usesecond=useseconda[0];}
	usesecond = a;
	usecross = b;
	//if(nrhs>3){ usecrossa = (bool*)mxGetPr(prhs[3]); usecross=usecrossa[0];}

	/* Create the distance output array */
	distanceMap = mxCreateDoubleMatrix(grid_size,grid_size,mxREAL);
	/* Assign pointer to output.  */
	/*Distance image, also used to store the index of narrowband pixels  */
	/*during marching process  */
	T= mxGetPr(distanceMap);

	/* Pixels which are processed and have a final distance are frozen */
	Frozen = (bool*)malloc( npixels* sizeof(bool) );
	for(q=0;q<npixels;q++){Frozen[q]=0; T[q]=-1;}

	/*Free memory to store neighbours of the (segmented) region */
	neg_free = 1000000;
	neg_pos=0;
	neg_listx = (double *)malloc( neg_free*sizeof(double) );
	neg_listy = (double *)malloc( neg_free*sizeof(double) );

	/* List parameters array */
	listprop=(int*)malloc(3* sizeof(int));
	/* Make jagged list to store a maximum of 2^64 values */
	listval= (double **)malloc( 64* sizeof(double *) );
	/* Initialize parameter list */
	initialize_list(listval, listprop);
	neg_listv=listval[listprop[1]-1];


	/*(There are 3 pixel classes:
	*  - frozen (processed)
	*  - narrow band (boundary) (in list to check for the next pixel with smallest distance)
	*  - far (not yet used)
	*/


	/* set all starting points to distance zero and frozen  */
	/* and add all neighbours of the starting points to narrow list  */

	/*for (z=0; z<1; z++) */
	for (z=0; z<dims_sp[1]; z++) 
	{
		/*starting point  */
		x= (int)SourcePoints[0+z*2]-1;
		y= (int)SourcePoints[1+z*2]-1;
		XY_index=x+y*dims[0];

		/*Set starting point to frozen and distance to zero  */
		Frozen[XY_index]=1;
		T[XY_index]=0;
	}

	for (z=0; z<dims_sp[1]; z++) {
		/*starting point  */
		x= (int)SourcePoints[0+z*2]-1;
		y= (int)SourcePoints[1+z*2]-1;
		XY_index=x+y*dims[0];

		/* Add neigbours of starting points  */
		for (k=0; k<4; k++) {
			/*Location of neighbour  */
			i=x+ne[k];
			j=y+ne[k+4];
			IJ_index=i+j*dims[0];

			/*Check if current neighbour is not yet frozen and inside the
			*picture  */
			if(isntfrozen2d(i,j, dims, Frozen)) {
				Tt=(1/(F[IJ_index]+eps));
				/*Update distance in neigbour list or add to neigbour list */
				if(T[IJ_index]>0) {
					if(neg_listv[(int)T[IJ_index]]>Tt) {
						listupdate(listval, listprop, (int)T[IJ_index], Tt);
					}
				}
				else {
					/*If running out of memory at a new block  */
					if(neg_pos>=neg_free) {
						neg_free+=100000;
						neg_listx = (double *)realloc(neg_listx, neg_free*sizeof(double) );
						neg_listy = (double *)realloc(neg_listy, neg_free*sizeof(double) );
					}
					list_add(listval, listprop, Tt);
					neg_listv=listval[listprop[1]-1];
					neg_listx[neg_pos]=i;
					neg_listy[neg_pos]=j;
					T[IJ_index]=neg_pos;
					neg_pos++;
				}
			}
		}
	}

	/*Loop through all pixels of the image  */
	for (itt=0; itt<npixels; itt++)
	{
		/*Get the pixel from narrow list (boundary list) with smallest
		*distance value and set it to current pixel location  */
		index=list_minimum(listval, listprop);
		neg_listv=listval[listprop[1]-1];    
		/* Stop if pixel distance is infinite (all pixels are processed)  */
		if(IsInf(neg_listv[index])) { break; }
		x=(int)neg_listx[index]; y=(int)neg_listy[index];

		XY_index=x+y*dims[0];
		Frozen[XY_index]=1;
		T[XY_index]=neg_listv[index];


		/*Remove min value by replacing it with the last value in the array  */
		list_remove_replace(listval, listprop, index) ;
		neg_listv=listval[listprop[1]-1];
		if(index<(neg_pos-1)) {
			neg_listx[index]=neg_listx[neg_pos-1];
			neg_listy[index]=neg_listy[neg_pos-1];
			T[(int)(neg_listx[index]+neg_listy[index]*dims[0])]=index;
		}
		neg_pos =neg_pos-1;

		/*Loop through all 4 neighbours of current pixel  */
		for (k=0;k<4;k++) {

			/*Location of neighbour  */
			i=x+ne[k]; j=y+ne[k+4];
			IJ_index=i+j*dims[0];

			/*Check if current neighbour is not yet frozen and inside the  */
			/*picture  */
			if(isntfrozen2d(i,j, dims, Frozen)) {

				/*Get First order derivatives (only use frozen pixel)  */
				in=i-1; jn=j+0; if(isfrozen2d(in, jn, dims, Frozen)) { Txm1=T[mindex2(in,jn,dims[0])]; } else { Txm1=INF; }
				in=i+0; jn=j-1; if(isfrozen2d(in, jn, dims, Frozen)) { Tym1=T[mindex2(in,jn,dims[0])]; } else { Tym1=INF; }
				in=i+0; jn=j+1; if(isfrozen2d(in, jn, dims, Frozen)) { Typ1=T[mindex2(in,jn,dims[0])]; } else { Typ1=INF; }
				in=i+1; jn=j+0; if(isfrozen2d(in, jn, dims, Frozen)) { Txp1=T[mindex2(in,jn,dims[0])]; } else { Txp1=INF; }
				if(usecross) {
					in=i-1; jn=j-1; if(isfrozen2d(in, jn, dims, Frozen)) { Tr1m1=T[mindex2(in,jn,dims[0])]; } else { Tr1m1=INF; }
					in=i-1; jn=j+1; if(isfrozen2d(in, jn, dims, Frozen)) { Tr2m1=T[mindex2(in,jn,dims[0])]; } else { Tr2m1=INF; }
					in=i+1; jn=j-1; if(isfrozen2d(in, jn, dims, Frozen)) { Tr2p1=T[mindex2(in,jn,dims[0])]; } else { Tr2p1=INF; }
					in=i+1; jn=j+1; if(isfrozen2d(in, jn, dims, Frozen)) { Tr1p1=T[mindex2(in,jn,dims[0])]; } else { Tr1p1=INF; }
				}
				/*The values in order is 0 if no neighbours in that direction  */
				/*1 if 1e order derivatives is used and 2 if second order  */
				/*derivatives are used  */
				Order[0]=0; Order[1]=0; Order[2]=0; Order[3]=0;

				/*Make 1e order derivatives in x and y direction  */
				Tm[0] = min( Txm1 , Txp1); if(IsFinite(Tm[0])){ Order[0]=1; }
				Tm[1] = min( Tym1 , Typ1); if(IsFinite(Tm[1])){ Order[1]=1; }
				/*Make 1e order derivatives in cross directions  */
				if(usecross) {
					Tm[2] = min( Tr1m1 , Tr1p1); if(IsFinite(Tm[2])){ Order[2]=1; }
					Tm[3] = min( Tr2m1 , Tr2p1); if(IsFinite(Tm[3])){ Order[3]=1; }
				}
				/*Make 2e order derivatives  */
				if(usesecond) {
					/*Get Second order derivatives (only use frozen pixel) */
					in=i-2; jn=j+0; if(isfrozen2d(in, jn, dims, Frozen)) { Txm2=T[mindex2(in,jn,dims[0])]; } else { Txm2=INF; }
					in=i+2; jn=j+0; if(isfrozen2d(in, jn, dims, Frozen)) { Txp2=T[mindex2(in,jn,dims[0])]; } else { Txp2=INF; }
					in=i+0; jn=j-2; if(isfrozen2d(in, jn, dims, Frozen)) { Tym2=T[mindex2(in,jn,dims[0])]; } else { Tym2=INF; }
					in=i+0; jn=j+2; if(isfrozen2d(in, jn, dims, Frozen)) { Typ2=T[mindex2(in,jn,dims[0])]; } else { Typ2=INF; }
					if(usecross) {                
						in=i-2; jn=j-2; if(isfrozen2d(in, jn, dims, Frozen)) { Tr1m2=T[mindex2(in,jn,dims[0])]; } else { Tr1m2=INF; }
						in=i-2; jn=j+2; if(isfrozen2d(in, jn, dims, Frozen)) { Tr2m2=T[mindex2(in,jn,dims[0])]; } else { Tr2m2=INF; }
						in=i+2; jn=j-2; if(isfrozen2d(in, jn, dims, Frozen)) { Tr2p2=T[mindex2(in,jn,dims[0])]; } else { Tr2p2=INF; }
						in=i+2; jn=j+2; if(isfrozen2d(in, jn, dims, Frozen)) { Tr1p2=T[mindex2(in,jn,dims[0])]; } else { Tr1p2=INF; }
					}

					Tm2[0]=0; Tm2[1]=0;Tm2[2]=0; Tm2[3]=0;
					/*pixels with a pixeldistance 2 from the center must be */
					/*lower in value otherwise use other side or first order */
					ch1=(Txm2<Txm1)&&IsFinite(Txm1); ch2=(Txp2<Txp1)&&IsFinite(Txp1);

					if(ch1&&ch2) {
						Tm2[0] =min( (4.0*Txm1-Txm2)/3.0 , (4.0*Txp1-Txp2)/3.0);  Order[0]=2;
					}
					else if (ch1) {
						Tm2[0]=(4.0*Txm1-Txm2)/3.0; Order[0]=2;
					}
					else if(ch2) {
						Tm2[0] =(4.0*Txp1-Txp2)/3.0; Order[0]=2;
					}

					ch1=(Tym2<Tym1)&&IsFinite(Tym1); ch2=(Typ2<Typ1)&&IsFinite(Typ1);

					if(ch1&&ch2) {
						Tm2[1] =min( (4.0*Tym1-Tym2)/3.0 , (4.0*Typ1-Typ2)/3.0); Order[1]=2;
					}
					else if(ch1) {
						Tm2[1]=(4.0*Tym1-Tym2)/3.0; Order[1]=2;
					}
					else if(ch2) {
						Tm2[1]=(4.0*Typ1-Typ2)/3.0; Order[1]=2;
					}
					if(usecross) {
						ch1=(Tr1m2<Tr1m1)&&IsFinite(Tr1m1); ch2=(Tr1p2<Tr1p1)&&IsFinite(Tr1p1);
						if(ch1&&ch2) {
							Tm2[2] =min( (4.0*Tr1m1-Tr1m2)/3.0 , (4.0*Tr1p1-Tr1p2)/3.0); Order[2]=2;
						}
						else if(ch1) {
							Tm2[2]=(4.0*Tr1m1-Tr1m2)/3.0; Order[2]=2;
						}
						else if(ch2){
							Tm2[2]=(4.0*Tr1p1-Tr1p2)/3.0; Order[2]=2;
						}

						ch1=(Tr2m2<Tr2m1)&&IsFinite(Tr2m1); ch2=(Tr2p2<Tr2p1)&&IsFinite(Tr2p1);
						if(ch1&&ch2){
							Tm2[3] =min( (4.0*Tr2m1-Tr2m2)/3.0 , (4.0*Tr2p1-Tr2p2)/3.0); Order[3]=2;
						}
						else if(ch1) {
							Tm2[3]=(4.0*Tr2m1-Tr2m2)/3.0; Order[3]=2;
						}
						else if(ch2) {
							Tm2[3]=(4.0*Tr2p1-Tr2p2)/3.0; Order[3]=2;
						}
					}
				}

				/*Calculate the distance using x and y direction */
				Coeff[0]=0; Coeff[1]=0; Coeff[2]=-1/(pow2(F[IJ_index])+eps);

				for (t=0; t<2; t++) {
					switch(Order[t]) {
						case 1:
							Coeff[0]+=1; Coeff[1]+=-2*Tm[t]; Coeff[2]+=pow2(Tm[t]);
							break;
						case 2:
							Coeff[0]+=(2.25); Coeff[1]+=-2*Tm2[t]*(2.25); Coeff[2]+=pow2(Tm2[t])*(2.25);
							break;
					}
				}
				roots(Coeff, ansroot);
				Tt=max(ansroot[0], ansroot[1]);

				/*Calculate the distance using the cross directions */
				if(usecross) {
					Coeff[0]=0; Coeff[1]=0; Coeff[2]=-1/(pow2(F[IJ_index])+eps);
					for (t=2; t<4; t++) {
						switch(Order[t]) {
							case 1:
								Coeff[0]+=c1; Coeff[1]+=-2.0*c1*Tm[t]; Coeff[2]+=c1*pow2(Tm[t]);
								break;
							case 2:
								Coeff[0]+=c1*2.25; Coeff[1]+=-2*c1*Tm2[t]*(2.25); Coeff[2]+=pow2(Tm2[t])*c1*2.25;
								break;
						}
					}
					if(Coeff[0]>0) {
						roots(Coeff, ansroot);
						Tt2=max(ansroot[0], ansroot[1]);
						/*Select minimum distance value of both stensils */
						Tt=min(Tt, Tt2);
					}
				}
				/*Upwind condition check, current distance must be larger */
				/*then direct neighbours used in solution */
				/*(Will this ever happen?) */
				if(usecross) {
					for(q=0; q<4; q++) { if(IsFinite(Tm[q])&&(Tt<Tm[q])) { Tt=Tm[minarray(Tm, 4)]+(1/(F[IJ_index]+eps));}}
				}
				else
				{
					for(q=0; q<2; q++) { if(IsFinite(Tm[q])&&(Tt<Tm[q])) { Tt=Tm[minarray(Tm, 2)]+(1/(F[IJ_index]+eps));}}
				}

				/*Update distance in neigbour list or add to neigbour list */
				IJ_index=i+j*dims[0];
				if((T[IJ_index]>-1)&&T[IJ_index]<=listprop[0]) {
					if(neg_listv[(int)T[IJ_index]]>Tt) {
						listupdate(listval, listprop,    (int)T[IJ_index], Tt);
					}
				}
				else {
					/*If running out of memory at a new block */
					if(neg_pos>=neg_free) {
						neg_free+=100000;
						neg_listx = (double *)realloc(neg_listx, neg_free*sizeof(double) );
						neg_listy = (double *)realloc(neg_listy, neg_free*sizeof(double) );
					}
					list_add(listval, listprop, Tt);
					neg_listv=listval[listprop[1]-1];
					neg_listx[neg_pos]=i; neg_listy[neg_pos]=j;
					T[IJ_index]=neg_pos;
					neg_pos++;
				}
			}
		}

	} 
	/* Free memory */
	/* Destroy parameter list */
	destroy_list(listval, listprop);
	free(neg_listx);
	free(neg_listy);
	free(Frozen);
	return distanceMap;
}

int FastMarching::mindex3(int x, int y, int z, int sizx, int sizy)  { return z*sizy*sizx+y*sizx+x;}


int FastMarching::checkBounds2d( double *point, int *Isize) {
	if((point[0]<0)||(point[1]<0)||(point[0]>(Isize[0]-1))||(point[1]>(Isize[1]-1))) { return false; }
	return true;
}

int FastMarching::checkBounds3d( double *point, int *Isize) {
	if((point[0]<0)||(point[1]<0)||(point[2]<0)||(point[0]>(Isize[0]-1))||(point[1]>(Isize[1]-1))||(point[2]>(Isize[2]-1))) { return false; }
	return true;
}

double FastMarching::norm2(double *a) { return sqrt(a[0]*a[0]+a[1]*a[1]); }
double FastMarching::norm3(double *a) { return sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]); }


void FastMarching::interpgrad2d(double *Ireturn, double *I, int *Isize, double *point) {
	/*  Linear interpolation variables */
	int xBas0, xBas1, yBas0, yBas1;
	double perc[4]={0, 0, 0, 0};
	double xCom, yCom, xComi, yComi;
	double fTlocalx, fTlocaly;
	int f;
	int index[4];

	fTlocalx = floor(point[0]); fTlocaly = floor(point[1]);
	xBas0=(int) fTlocalx; yBas0=(int) fTlocaly;
	xBas1=xBas0+1; yBas1=yBas0+1;

	/* Linear interpolation constants (percentages) */
	xCom=point[0]-fTlocalx; yCom=point[1]-fTlocaly;
	xComi=(1-xCom); yComi=(1-yCom);
	perc[0]=xComi * yComi;
	perc[1]=xComi * yCom;
	perc[2]=xCom * yComi;
	perc[3]=xCom * yCom;

	/* Stick to boundary */
	if(xBas0<0) { xBas0=0; if(xBas1<0) { xBas1=0; }}
	if(yBas0<0) { yBas0=0; if(yBas1<0) { yBas1=0; }}
	if(xBas1>(Isize[0]-1)) { xBas1=Isize[0]-1; if(xBas0>(Isize[0]-1)) { xBas0=Isize[0]-1; }}
	if(yBas1>(Isize[1]-1)) { yBas1=Isize[1]-1; if(yBas0>(Isize[1]-1)) { yBas0=Isize[1]-1; }}

	/* Get the neighbour intensities */
	index[0]=mindex2(xBas0, yBas0, Isize[0]);
	index[1]=mindex2(xBas0, yBas1, Isize[0]);
	index[2]=mindex2(xBas1, yBas0, Isize[0]);
	index[3]=mindex2(xBas1, yBas1, Isize[0]);
	f=Isize[0]*Isize[1];

	/* the interpolated color */
	Ireturn[0]=I[index[0]]*perc[0]+I[index[1]]*perc[1]+I[index[2]]*perc[2]+I[index[3]]*perc[3];
	Ireturn[1]=I[index[0]+f]*perc[0]+I[index[1]+f]*perc[1]+I[index[2]+f]*perc[2]+I[index[3]+f]*perc[3];
}

void FastMarching::interpgrad3d(double *Ireturn, double *I, int *Isize, double *point) {
	/*  Linear interpolation variables */
	int xBas0, xBas1, yBas0, yBas1, zBas0, zBas1;
	double perc[8];
	double xCom, yCom, zCom;
	double xComi, yComi, zComi;
	double fTlocalx, fTlocaly, fTlocalz;
	int f0, f1;
	int index[8];
	double temp;

	fTlocalx = floor(point[0]); fTlocaly = floor(point[1]); fTlocalz = floor(point[2]);
	xBas0=(int) fTlocalx; yBas0=(int) fTlocaly; zBas0=(int) fTlocalz;
	xBas1=xBas0+1; yBas1=yBas0+1; zBas1=zBas0+1;

	/* Linear interpolation constants (percentages) */
	xCom=point[0]-fTlocalx;  yCom=point[1]-fTlocaly;   zCom=point[2]-fTlocalz;
	xComi=(1-xCom); yComi=(1-yCom); zComi=(1-zCom);
	perc[0]=xComi * yComi; perc[1]=perc[0] * zCom; perc[0]=perc[0] * zComi;
	perc[2]=xComi * yCom;  perc[3]=perc[2] * zCom; perc[2]=perc[2] * zComi;
	perc[4]=xCom * yComi;  perc[5]=perc[4] * zCom; perc[4]=perc[4] * zComi;
	perc[6]=xCom * yCom;   perc[7]=perc[6] * zCom; perc[6]=perc[6] * zComi;

	/* Stick to boundary */
	if(xBas0<0) { xBas0=0; if(xBas1<0) { xBas1=0; }}
	if(yBas0<0) { yBas0=0; if(yBas1<0) { yBas1=0; }}
	if(zBas0<0) { zBas0=0; if(zBas1<0) { zBas1=0; }}
	if(xBas1>(Isize[0]-1)) { xBas1=Isize[0]-1; if(xBas0>(Isize[0]-1)) { xBas0=Isize[0]-1; }}
	if(yBas1>(Isize[1]-1)) { yBas1=Isize[1]-1; if(yBas0>(Isize[1]-1)) { yBas0=Isize[1]-1; }}
	if(zBas1>(Isize[2]-1)) { zBas1=Isize[2]-1; if(zBas0>(Isize[2]-1)) { zBas0=Isize[2]-1; }}

	/*Get the neighbour intensities */
	index[0]=mindex3(xBas0, yBas0, zBas0, Isize[0], Isize[1]);
	index[1]=mindex3(xBas0, yBas0, zBas1, Isize[0], Isize[1]);
	index[2]=mindex3(xBas0, yBas1, zBas0, Isize[0], Isize[1]);
	index[3]=mindex3(xBas0, yBas1, zBas1, Isize[0], Isize[1]);
	index[4]=mindex3(xBas1, yBas0, zBas0, Isize[0], Isize[1]);
	index[5]=mindex3(xBas1, yBas0, zBas1, Isize[0], Isize[1]);
	index[6]=mindex3(xBas1, yBas1, zBas0, Isize[0], Isize[1]);
	index[7]=mindex3(xBas1, yBas1, zBas1, Isize[0], Isize[1]);
	f0=Isize[0]*Isize[1]*Isize[2];
	f1=f0+f0;

	/*the interpolated color */
	temp=I[index[0]]*perc[0]+I[index[1]]*perc[1]+I[index[2]]*perc[2]+I[index[3]]*perc[3];
	Ireturn[0]=temp+I[index[4]]*perc[4]+I[index[5]]*perc[5]+I[index[6]]*perc[6]+I[index[7]]*perc[7];
	temp=I[index[0]+f0]*perc[0]+I[index[1]+f0]*perc[1]+I[index[2]+f0]*perc[2]+I[index[3]+f0]*perc[3];
	Ireturn[1]=temp+I[index[4]+f0]*perc[4]+I[index[5]+f0]*perc[5]+I[index[6]+f0]*perc[6]+I[index[7]+f0]*perc[7];
	temp=I[index[0]+f1]*perc[0]+I[index[1]+f1]*perc[1]+I[index[2]+f1]*perc[2]+I[index[3]+f1]*perc[3];
	Ireturn[2]=temp+I[index[4]+f1]*perc[4]+I[index[5]+f1]*perc[5]+I[index[6]+f1]*perc[6]+I[index[7]+f1]*perc[7];
}

bool FastMarching::RK4STEP_2D(double *gradientArray, int *gradientArraySize, double *startPoint, double *nextPoint, double stepSize) {
	/* Perform one step of the RK4 algorithm */
	double k1[2], k2[2], k3[2], k4[2];
	double tempPoint[2];
	double tempnorm;

	/*Calculate k1 */
	interpgrad2d(k1, gradientArray, gradientArraySize, startPoint);
	tempnorm=norm2(k1);
	k1[0] = k1[0]*stepSize/tempnorm;
	k1[1] = k1[1]*stepSize/tempnorm;

	tempPoint[0]=startPoint[0] - k1[0]*0.5;
	tempPoint[1]=startPoint[1] - k1[1]*0.5;

	/*Check the if are still inside the domain */
	if (!checkBounds2d(tempPoint, gradientArraySize)) return false;

	/*Calculate k2 */
	interpgrad2d(k2, gradientArray, gradientArraySize, tempPoint);
	tempnorm=norm2(k2);
	k2[0] = k2[0]*stepSize/tempnorm;
	k2[1] = k2[1]*stepSize/tempnorm;

	tempPoint[0]=startPoint[0] - k2[0]*0.5;
	tempPoint[1]=startPoint[1] - k2[1]*0.5;

	/*Check the if are still inside the domain */
	if (!checkBounds2d(tempPoint, gradientArraySize)) return false;

	/*Calculate k3 */
	interpgrad2d(k3, gradientArray, gradientArraySize, tempPoint);
	tempnorm=norm2(k3);
	k3[0] = k3[0]*stepSize/tempnorm;
	k3[1] = k3[1]*stepSize/tempnorm;

	tempPoint[0]=startPoint[0] - k3[0];
	tempPoint[1]=startPoint[1] - k3[1];

	double a = startPoint[0];
	double b = startPoint[1];

	a = nextPoint[0];
	b = nextPoint[1];

	/*Check the if are still inside the domain */
	if (!checkBounds2d(tempPoint, gradientArraySize)) return false;

	/*Calculate k4 */
	interpgrad2d(k4, gradientArray, gradientArraySize, tempPoint);
	tempnorm=norm2(k4);
	k4[0] = k4[0]*stepSize/tempnorm;
	k4[1] = k4[1]*stepSize/tempnorm;

	/*Calculate final point */
	nextPoint[0] = startPoint[0] - (k1[0] + k2[0]*2.0 + k3[0]*2.0 + k4[0])/6.0;
	nextPoint[1] = startPoint[1] - (k1[1] + k2[1]*2.0 + k3[1]*2.0 + k4[1])/6.0;

	/*Check the if are still inside the domain */
	if (!checkBounds2d(nextPoint, gradientArraySize)) return false;

	return true;
}


bool FastMarching::RK4STEP_3D(double *gradientArray, int *gradientArraySize, double *startPoint, double *nextPoint, double stepSize) {
	double k1[3], k2[3], k3[3], k4[3];
	double tempPoint[3];
	double tempnorm;

	/*Calculate k1 */
	interpgrad3d(k1, gradientArray, gradientArraySize, startPoint);
	tempnorm=norm3(k1);
	k1[0] = k1[0]*stepSize/tempnorm;
	k1[1] = k1[1]*stepSize/tempnorm;
	k1[2] = k1[2]*stepSize/tempnorm;

	tempPoint[0]=startPoint[0] - k1[0]*0.5;
	tempPoint[1]=startPoint[1] - k1[1]*0.5;
	tempPoint[2]=startPoint[2] - k1[2]*0.5;

	/*Check the if are still inside the domain */
	if (!checkBounds3d(tempPoint, gradientArraySize)) return false;

	/*Calculate k2 */
	interpgrad3d(k2, gradientArray, gradientArraySize, tempPoint);
	tempnorm=norm3(k2);
	k2[0] = k2[0]*stepSize/tempnorm;
	k2[1] = k2[1]*stepSize/tempnorm;
	k2[2] = k2[2]*stepSize/tempnorm;

	tempPoint[0]=startPoint[0] - k2[0]*0.5;
	tempPoint[1]=startPoint[1] - k2[1]*0.5;
	tempPoint[2]=startPoint[2] - k2[2]*0.5;

	/*Check the if are still inside the domain */
	if (!checkBounds3d(tempPoint, gradientArraySize)) return false;

	/*Calculate k3 */
	interpgrad3d(k3, gradientArray, gradientArraySize, tempPoint);
	tempnorm=norm3(k3);
	k3[0] = k3[0]*stepSize/tempnorm;
	k3[1] = k3[1]*stepSize/tempnorm;
	k3[2] = k3[2]*stepSize/tempnorm;

	tempPoint[0]=startPoint[0] - k3[0];
	tempPoint[1]=startPoint[1] - k3[1];
	tempPoint[2]=startPoint[2] - k3[2];

	/*Check the if are still inside the domain */
	if (!checkBounds3d(tempPoint, gradientArraySize)) return false;

	/*Calculate k4 */
	interpgrad3d(k4, gradientArray, gradientArraySize, tempPoint);
	tempnorm=norm3(k4);
	k4[0] = k4[0]*stepSize/tempnorm;
	k4[1] = k4[1]*stepSize/tempnorm;
	k4[2] = k4[2]*stepSize/tempnorm;

	/*Calculate final point */
	nextPoint[0] = startPoint[0] - (k1[0] + k2[0]*2.0 + k3[0]*2.0 + k4[0])/6.0;
	nextPoint[1] = startPoint[1] - (k1[1] + k2[1]*2.0 + k3[1]*2.0 + k4[1])/6.0;
	nextPoint[2] = startPoint[2] - (k1[2] + k2[2]*2.0 + k3[2]*2.0 + k4[2])/6.0;

	/*Check the if are still inside the domain */
	if (!checkBounds3d(nextPoint, gradientArraySize)) return false;

	return true;
}

double* FastMarching::rk4(mxArray* StartPoint, Mm GradientVolume, double Stepsize) {

	int gradientArraySizeC[3];
	const int *PointSizeC;
	int gradientArraySize[3];
	int PointDims;
	int gradientDims;
	int PointLength=1;
	double *startPoint;
	double startPoint1[3];
	double *nextPoint;
	double stepSize;
	double *stepSizeArray;
	int i;

	/* Check for proper number of input and output arguments. */
	//if(nrhs!=3) { mexErrMsgTxt("3 inputs are required."); }
	//if(nlhs!=1) { mexErrMsgTxt("One output required"); }

	/*  Get the number of gradient dimensions */
	//gradientDims=mxGetNumberOfDimensions(GradientVolume);
	gradientDims = 3;

	/*Get the size of the gradient Array */
	//gradientArraySizeC = mxGetDimensions(GradientVolume);
	gradientArraySizeC[0] = gradientArraySizeC[1] = grid_size;
	gradientArraySizeC[2] = 2;

	/*  Get the number of startingpoint dimensions */
	PointDims=mxGetNumberOfDimensions(StartPoint);

	//cout<<"PointDims= "<<PointDims<<endl;
	/*Get the size of the startingpoint */
	PointSizeC = mxGetDimensions(StartPoint);

	//cout<<"PointSizeC[0]= "<<PointSizeC[0]<<" "<<PointSizeC[1]<<endl;
	for (i=0; i<PointDims; i++) { PointLength=PointLength*PointSizeC[i]; }

	/*Check data input types  */
	//if(mxGetClassID(prhs[0])!=mxDOUBLE_CLASS) { mexErrMsgTxt("inputs must be of class double"); }
	//if(mxGetClassID(prhs[1])!=mxDOUBLE_CLASS) { mexErrMsgTxt("inputs must be of class double"); }
	//if(mxGetClassID(prhs[2])!=mxDOUBLE_CLASS) { mexErrMsgTxt("inputs must be of class double"); }

	/*Connect inputs */
	startPoint =  mxGetPr(StartPoint);

	//cout<<"startPoint[0]= "<<startPoint[0]<<" "<<startPoint[1]<<endl;

	double *gradientArray = new double[2*grid_size*grid_size];
	//gradientArray = mxGetPr(GradientVolume);
	memcpy(gradientArray,GradientVolume.addr(),2*grid_size*grid_size*sizeof(double));


	//stepSizeArray = mxGetPr(prhs[2]); stepSize=stepSizeArray[0];
	stepSize= Stepsize;

	/*Create the output array */
	//mxArray* EndPoint = mxCreateNumericArray(2, PointSizeC, mxDOUBLE_CLASS, mxREAL);
	mxArray* EndPoint = mxCreateDoubleMatrix(1,2,mxREAL);
	nextPoint= mxGetPr(EndPoint);

	/*Perform the RK4 raytracing step */
	if(PointLength==2) {
		gradientArraySize[0]=gradientArraySizeC[0];
		gradientArraySize[1]=gradientArraySizeC[1];
		startPoint1[0]=startPoint[0]-1; startPoint1[1]=startPoint[1]-1;
		if(RK4STEP_2D(gradientArray, gradientArraySize, startPoint1, nextPoint, stepSize)) {
			nextPoint[0]++; nextPoint[1]++;
		}
		else {
			nextPoint[0]=0; nextPoint[1]=0;
		}
	}
	else if(PointLength==3) {
		gradientArraySize[0]=gradientArraySizeC[0];
		gradientArraySize[1]=gradientArraySizeC[1];
		gradientArraySize[2]=gradientArraySizeC[2];
		startPoint1[0]=startPoint[0]-1; startPoint1[1]=startPoint[1]-1; startPoint1[2]=startPoint[2]-1;
		if(RK4STEP_3D(gradientArray, gradientArraySize, startPoint1, nextPoint, stepSize)) {
			nextPoint[0]++; nextPoint[1]++; nextPoint[2]++;
		}
		else {
			nextPoint[0]=0; nextPoint[1]=0; nextPoint[2]=0;
		}

	}
	else {
		mexErrMsgTxt("Starting Point must be 2D or 3D");
	}

	double a = nextPoint[0];
	double b = nextPoint[1];
	//cout<<"EndPoint"<<nextPoint[0]<<" "<<nextPoint[1]<<endl;
	return nextPoint;
}


vector<D3DXVECTOR3> FastMarching::fmm(double start_x,double start_y,double end_x,double end_y,CString txt)
{
	mxArray *SpeedImage = NULL;
	mxArray *distanceMap = NULL;
	SpeedImage = mxCreateDoubleMatrix(grid_size,grid_size,mxREAL);
	distanceMap = mxCreateDoubleMatrix(grid_size,grid_size,mxREAL);

	ifstream readFile(txt);
	double *T= (double*)mxGetPr(SpeedImage);
	for (int i=0;i<grid_size;i++)
	{
		for (int j=0;j<grid_size;j++)
		{
			readFile>>T[i*grid_size+j];
		}
	}

	readFile.close();

	int kkk= start_x*grid_size+start_y;

	if(T[kkk] == 0)
		start_x = start_x+1;

	kkk = end_x*grid_size+end_y;
	if(T[kkk]==0)
	{
		end_x = end_x+1;
		end_y = end_y +1;
	}

	//ofstream writeFile1("D:\grids1111.txt");
	////打印出grid
	//writeFile1<<"grid"<<endl;
	//for(int i=0;i<grid_size;i++)
	//{
	//	for (int j=0;j<grid_size;j++)
	//	{
	//		writeFile1<<T[i*grid_size+j]<<" ";
	//	}
	//	writeFile1<<endl;
	//}

	//mlfAssign(&SpeedImage,mlfDoubleMatrix(2,3,real_data,NULL));

	for (int i=0;i<=grid_size;i++)
	{
		for (int j=0;j<grid_size;j++)
		{
			T[i*grid_size+j] = T[i*grid_size+j]*1000+0.001;
		}	
	}
	mxArray *SourcePoint;
	SourcePoint = mxCreateDoubleMatrix(2,2,mxREAL);
	double *S= mxGetPr(SourcePoint);
	S[0] = end_x;
	S[1] = end_y;
	int a = S[0]*grid_size+S[1];
	if(T[a] == 0)
		MessageBox(NULL,_T("The destination point collise with the obstacle! "),_T("Error"),MB_OK);
	mxArray *StartPoint;
	StartPoint = mxCreateDoubleMatrix(1,2,mxREAL);
	double *Start= mxGetPr(StartPoint);
	Start[0] = start_x;
	Start[1] =start_y;
	a = Start[0]*grid_size+S[1];
	if(T[a] == 0)
		MessageBox(NULL,_T("The start point collise with the obstacle! "),_T("Error"),MB_OK);
	distanceMap = msfm2d(SpeedImage,SourcePoint,false,false);
	double *D= mxGetPr(distanceMap);

	//ofstream writeFile2("D:\grids2.txt");
	////打印出DistanceMap
	//writeFile2<<"DistanceMap"<<endl;
	//for(int i=0;i<grid_size;i++)
	//{
	//	for (int j=0;j<grid_size;j++)
	//	{
	//		writeFile2<<D[i*grid_size+j]<<" ";
	//	}
	//	writeFile2<<endl;
	//}
	//writeFile2.close();

	Mm k = zeros(grid_size,grid_size);
	Mm hx = zeros(grid_size,grid_size);
	Mm hy = zeros(grid_size,grid_size);
	memcpy(k.addr(),D,grid_size*grid_size*sizeof(double));
	hy = gradient(k);
	hx = gradient(transpose(k));
	hx = transpose(hx);
	//hy = transpose(hy);

	//ofstream writeFile3("D:\grids3.txt");
	////打印出grid
	//writeFile3<<"hx"<<endl;
	//for(int i=1;i<=grid_size;i++)
	//{
	//	for (int j=1;j<=grid_size;j++)
	//	{
	//		writeFile3<<hx.r(i,j)<<" ";
	//	}
	//	writeFile3<<endl;
	//}

	//ofstream writeFile4("D:\grids4.txt");
	////打印出grid
	//writeFile4<<"hy"<<endl;
	//for(int i=1;i<=grid_size;i++)
	//{
	//	for (int j=1;j<=grid_size;j++)
	//	{
	//		writeFile4<<hy.r(i,j)<<" ";
	//	}
	//	writeFile4<<endl;
	//}
	//writeFile4.close();

	Mm GradientVolume = zeros(grid_size,grid_size,2);

	for (int i=1;i<=grid_size;i++)
	{
		for (int j=1;j<=grid_size;j++)
		{
			GradientVolume(i,j,1) = hx(i,j); 
			GradientVolume(i,j,2) = hy(i,j); 
		}
	}


	int timeCounter = 0;
	int ifree = grid_size*grid_size;
	//Mm ShortestLine = zeros(ifree,2);
	vector<D3DXVECTOR3> ShortestLine;
	bool isend = true;
	double DistancetoEnd;
	mxArray *EndPoint;
	EndPoint = mxCreateDoubleMatrix(1,2,mxREAL);
	double *End= mxGetPr(EndPoint);

	
	double Movement;
	double p,q;
	D3DXVECTOR3 tt;
	while (isend)
	{
		End = rk4(StartPoint,GradientVolume,0.5);
		p = End[0];
		q = End[1];

		DistancetoEnd = sqrt((S[0]-Start[0])*(S[0]-Start[0])+(S[1]-Start[1])*(S[1]-Start[1]));
		if (timeCounter>10)
			Movement = sqrt(((End[0]-ShortestLine[timeCounter-10].x)*(End[0]-ShortestLine[timeCounter-10].x)+(End[1]-ShortestLine[timeCounter-10].y)*(End[1]-ShortestLine[timeCounter-10].y)));
		else
			Movement = 1;

		if((End[0] == 0) || (Movement<0.5))
			break;

		timeCounter++;
		/*if (timeCounter>ifree)
		{
			ifree += 10000;
			ShortestLine.reshape(ifree,2);
		}*/

		tt.x = End[0];
		tt.y = End[1];
		ShortestLine.push_back(tt);

		if (DistancetoEnd < 0.5)
		{
			timeCounter++;
			/*if (timeCounter>ifree)
			{
				ifree = ifree + 10000;
				ShortestLine.reshape(ifree,2);
			}*/
			/*ShortestLine(timeCounter,1)= S[0];
			ShortestLine(timeCounter,2)= S[1];*/
			tt.x = S[0];
			tt.y = S[1];
			ShortestLine.push_back(tt);
			break;
		}

		Start[0] = End[0];
		Start[1] = End[1];

	}

	//ShortestLine = 


	//GradientVolume = mxCreateDoubleMatrix(100,100,2,mxREAL);
	//ofstream writeFile4("E:\grids4.txt");
	//writeFile4<<"ShortestLine:"<<endl;
	/*for(int i=1;i<=200;i++)
	{
	cout<<ShortestLine.r(i,1)<<" ";
	cout<<ShortestLine.r(i,2)<<endl;
	}*/

	//writeFile4.close();
	//ofstream writeFile5("D:\grids5.txt");
	////打印出grid
	//writeFile5<<"ShortestLine"<<endl;
	//int n = ShortestLine.size();
	//for(int i=0;i<n;i++)
	//{
	//	writeFile5<<ShortestLine[i].x<<" ";
	//	writeFile5<<ShortestLine[i].y<<endl;
	//}

	//writeFile5.close();
	//delete(GradientVolume);
	/*free(Start);
	free(End);
	mxDestroyArray(distanceMap);
	mxDestroyArray(SourcePoint);
	mxDestroyArray(StartPoint);
	mxDestroyArray(SpeedImage);
	clear(CL(GradientVolume));*/

	/*ifstream rr("D:\grids.txt");
	int gg[GRIDNUM][GRIDNUM];
	for (int i=0;i<GRIDNUM;i++)
	{
		for (int j=0;j<GRIDNUM;j++)
		{
			rr>>gg[i][j];
		}
	}

	rr.close();
	int pp,qq;
	for (int i=0;i<ShortestLine.size();i++)
	{
		pp = (int)ShortestLine[i].x;
		qq = (int)ShortestLine[i].y;
		gg[pp][qq] = 2;
	}

	ofstream writeFile5("D:\ppqq.txt");
	for (int i=0;i<GRIDNUM;i++)
	{
		for (int j=0;j<GRIDNUM;j++)
		{
			writeFile5<<gg[i][j];
		}
		writeFile5<<endl;
	}
	writeFile5.close();*/
	return ShortestLine;

}

int FastMarching::getTimeCounter()
{
	return timeCounter;
}

void FastMarching::setGridSize(int grid_size)
{
	this->grid_size = grid_size;
}

int FastMarching::getGridSize()
{
	return grid_size;
}