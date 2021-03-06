/* s2poincare.c
 *
 * We would appreciate it if research outcomes using S2PLOT would
 * provide the following acknowledgement:
 *
 * "Three-dimensional visualisation was conducted with the S2PLOT
 * progamming library"
 *
 * and a reference to
 *
 * D.G.Barnes, C.J.Fluke, P.D.Bourke & O.T.Parry, 2006, Publications
 * of the Astronomical Society of Australia, 23(2), 82-93.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "s2plot.h"
extern int _s2_conelines;

#define STEP 100

struct Pn{
	float *xp;
	float *yp;
	float *zp;
	float *sp;
    int np;
};

struct Pts3d{
	float *xs;
	float *ys;
	float *zs;
    int ns;
};

struct Pts3d ReadPts3dFile(FILE *fin){
	struct Pts3d data;
	data.xs=NULL;
	data.ys=NULL;
	data.zs=NULL;
	data.ns=0;
	   float fx, fy, fz;
	   char line[256];
	   while (!feof(fin)) {
	     if (fgets(line, 255, fin)) {
	       if (sscanf(line, "%f%f%f", &fx, &fy, &fz) == 3) {
		 if (!(data.ns % STEP)) {
			 data.xs = (float *)realloc(data.xs, (data.ns+STEP) * sizeof(float));
			 data.ys = (float *)realloc(data.ys, (data.ns+STEP) * sizeof(float));
			 data.zs = (float *)realloc(data.zs, (data.ns+STEP) * sizeof(float));
		 }
		 data.xs[data.ns] = fx;
		 data.ys[data.ns] = fy;
		 data.zs[data.ns] = fz;
		 data.ns++;
	       } else {
		 fprintf(stderr, "Couldn't read 4 floats at line %d\n", data.ns);
		 exit(-1);
	       }
	     }
	   }
	   return data;
}



struct Pn ReadPnFile(FILE *fin){
	struct Pn data;
	data.xp=NULL;
	data.yp=NULL;
	data.zp=NULL;
	data.sp=NULL;
	data.np=0;
	   float fx, fy, fz, fs;
	   char line[256];
	   while (!feof(fin)) {
	     if (fgets(line, 255, fin)) {
	       if (sscanf(line, "%f%f%f%f", &fx, &fy, &fz, &fs) == 4) {
		 if (!(data.np % STEP)) {
			 data.xp = (float *)realloc(data.xp, (data.np+STEP) * sizeof(float));
			 data.yp = (float *)realloc(data.yp, (data.np+STEP) * sizeof(float));
			 data.zp = (float *)realloc(data.zp, (data.np+STEP) * sizeof(float));
			 data.sp = (float *)realloc(data.sp, (data.np+STEP) * sizeof(float));
		 }
		 data.xp[data.np] = fx;
		 data.yp[data.np] = fy;
		 data.zp[data.np] = fz;
		 data.sp[data.np] = fs;
		 data.np++;
	       } else {
		 fprintf(stderr, "Couldn't read 4 floats at line %d\n", data.np);
		 exit(-1);
	       }
	     }
	   }
	   return data;
}

struct Pts3d ReadSelectedPts3d(struct Pts3d ptstot, int i1, int i2){
	int i;
	struct Pts3d select;
	select.xs=NULL;
	select.ys=NULL;
	select.zs=NULL;
	select.ns=0;
	for(i=i1;i<i2;i=i+1){
		 if (!(select.ns % STEP)) {
			 select.xs = (float *)realloc(select.xs, (select.ns+STEP) * sizeof(float));
			 select.ys = (float *)realloc(select.ys, (select.ns+STEP) * sizeof(float));
			 select.zs = (float *)realloc(select.zs, (select.ns+STEP) * sizeof(float));
		 }
		select.xs[select.ns] = ptstot.xs[i];
		select.ys[select.ns] = ptstot.ys[i];
		select.zs[select.ns] = ptstot.zs[i];
		select.ns++;
	}
	return select;

}

struct Pts3d ReadSelectedPts3d_odd(struct Pts3d ptstot, int i1, int i2){
	int i;
	struct Pts3d select;
	select.xs=NULL;
	select.ys=NULL;
	select.zs=NULL;
	select.ns=0;
	for(i=i1;i<i2;i=i+2){
		 if (!(select.ns % STEP)) {
			 select.xs = (float *)realloc(select.xs, (select.ns+STEP) * sizeof(float));
			 select.ys = (float *)realloc(select.ys, (select.ns+STEP) * sizeof(float));
			 select.zs = (float *)realloc(select.zs, (select.ns+STEP) * sizeof(float));
		 }
		select.xs[select.ns] = ptstot.xs[i];
		select.ys[select.ns] = ptstot.ys[i];
		select.zs[select.ns] = ptstot.zs[i];
		select.ns++;
	}
	return select;

}

struct Pts3d ReadSelectedPts3d_even(struct Pts3d ptstot, int i1, int i2){
	int i;
	struct Pts3d select;
	select.xs=NULL;
	select.ys=NULL;
	select.zs=NULL;
	select.ns=0;
	for(i=i1+1;i<i2;i=i+2){
		 if (!(select.ns % STEP)) {
			 select.xs = (float *)realloc(select.xs, (select.ns+STEP) * sizeof(float));
			 select.ys = (float *)realloc(select.ys, (select.ns+STEP) * sizeof(float));
			 select.zs = (float *)realloc(select.zs, (select.ns+STEP) * sizeof(float));
		 }
		select.xs[select.ns] = ptstot.xs[i];
		select.ys[select.ns] = ptstot.ys[i];
		select.zs[select.ns] = ptstot.zs[i];
		select.ns++;
	}
	return select;

}

void plotPnstab_new_Pts(struct Pn P, int iec, int ihc, int idc, float pn_size, float pd_size){
	int i;
        
	for( i = 0; i < P.np; i = i + 1 ){
		   if(P.sp[i]==1.0){
                    s2sci(iec);
                    s2sch(pn_size);
                    s2pt1(P.xp[i], P.yp[i], P.zp[i], 4);
	           }
		   else if(P.sp[i]==0){
               s2sci(idc);
               s2sch(pd_size);
               s2pt1(P.xp[i], P.yp[i], P.zp[i], 4);
		   }
                   else {
                   s2sci(ihc);
                   s2sch(pn_size);
                    s2pt1(P.xp[i], P.yp[i], P.zp[i], 4);

		   }
      }
}

int main(int argc, char *argv[])
{
 //  int symbol = 1;				/* Shaded sphere symbol */
// ./p1_p2_lines_full_square_pnts_manifolds_track p1_pnts_beta_2.0.dat p2line.dat p1_pnts_re1_beta_2.0.dat p2pnt_track.dat beta_2_pt12_maniflds_forw_odd.dat beta_2_pt12_maniflds_forw_even.dat p1pnt508_full_square_track_forw_prds600.dat p1pnt508_full_square_track_backw_prds600.dat p1pnt513_full_square_track_forw_prds600.dat p1pnt513_full_square_track_backw_prds600.dat p1pnt518_full_square_track_forw_prds600.dat p1pnt518_full_square_track_backw_prds600.dat
	FILE *fin_p1 = fopen(argv[1], "r");
	struct Pn P1=ReadPnFile(fin_p1);
	FILE *fin_p2 = fopen(argv[2], "r");
	struct Pn P2=ReadPnFile(fin_p2);
	FILE *fin_p1re1 = fopen(argv[3], "r");
	struct Pn P1re1=ReadPnFile(fin_p1re1);

	
/*	FILE *pnts1 = fopen(argv[4], "r");
	struct Pts3d pts504f=ReadPts3dFile(pnts1);
	FILE *pnts2 = fopen(argv[5], "r");
	struct Pts3d pts504b=ReadPts3dFile(pnts2);
	FILE *pnts3 = fopen(argv[6], "r");
	struct Pts3d pts506f=ReadPts3dFile(pnts3);
	FILE *pnts4 = fopen(argv[7], "r");
	struct Pts3d pts506b=ReadPts3dFile(pnts4);
	FILE *pnts5 = fopen(argv[8], "r");
	struct Pts3d pts508f=ReadPts3dFile(pnts5);
	FILE *pnts6 = fopen(argv[9], "r");
	struct Pts3d pts508b=ReadPts3dFile(pnts6); */

	FILE *pnts9 = fopen(argv[4], "r");  
	struct Pts3d p2ptorb=ReadPts3dFile(pnts9);
	FILE *pnts10= fopen(argv[5], "r");
	struct Pts3d p1pt2orb_odd=ReadPts3dFile(pnts10);
	FILE *pnts11= fopen(argv[6], "r");
	struct Pts3d p1pt2orb_even=ReadPts3dFile(pnts11);
	
	FILE *pnts12 = fopen(argv[7], "r"); 
	struct Pts3d pts508f=ReadPts3dFile(pnts12);
		FILE *pnts13 = fopen(argv[8], "r");
	struct Pts3d pts508b=ReadPts3dFile(pnts13);
		
		FILE *pnts14 = fopen(argv[9], "r"); 
	struct Pts3d pts513f=ReadPts3dFile(pnts14);
	FILE *pnts15 = fopen(argv[10], "r");
	struct Pts3d pts513b=ReadPts3dFile(pnts15); 

	FILE *pnts7 = fopen(argv[11], "r"); 
	struct Pts3d pts518f=ReadPts3dFile(pnts7);
	FILE *pnts8 = fopen(argv[12], "r");
	struct Pts3d pts518b=ReadPts3dFile(pnts8); 
	
    FILE *pnts1 = fopen(argv[13], "r"); 
    struct Pts3d pts513f_s=ReadPts3dFile(pnts1);
	FILE *pnts2 = fopen(argv[14], "r");
	struct Pts3d pts513b_s=ReadPts3dFile(pnts2); 
	
	
	
   s2opend("/?",argc, argv);			/* Open the display */
   s2swin(-1.,1., 0,1., -1.,1.);		/* Set the window coordinates */
   s2svp(-1,1, 0,1, -1,1);
 /*  s2box("BCDET",0,0,"BCDET",0,0,"BCDET",0,0); */	/* Draw coordinate box */
   
//   s2sch(0.8);					/* Sets size of most symbols */

 /*  s2sci(S2_PG_BLACK); */

//   s2sci(7);
   s2sch(0.1);
   s2scr(18, 0.03137254901, 0.5725490196, 0.81568627451);  /* electric blue def */
  	s2scr(21, 0.50196078431, 1, 1);  /* maroon */
  	s2scr(22, 0.454901960784314,0.729411764705882,0.925490196078431);  /* saddle brown */
  	s2scr(23, 1-0,1-0.921568627450980,1-0.423529411764706);  /* deep pink */
  	s2scr(24, 1-0.498039215686275,1-1,1-0.498039215686275);  /* purple */
  	s2scr(26, 1-0.419607843137255,1-1,1-0.172549019607843);  /* dark violet */
  	s2scr(27, 1-0.705882352941176,1-1,1-0.490196078431373);  /* indigo */
  	s2scr(29, 1-1,1-1,1-0.498039215686275);  /*navy */
  	s2scr(30, 0.901960784313726,0.901960784313726,0.980392156862745);  /*lavender */
  	s2scr(31, 0.960784313725490,0.870588235294118,0.701960784313725);  /*wheat */
  	s2scr(32, 0.980392156862745,0.501960784313726,0.447058823529412);  /*salmon */
  	s2scr(33, 0.529411764705882,0.807843137254902,0.921568627450980);  /*sky blue */	
  	s2scr(34, 0.823529411764706,0.411764705882353,0.117647058823529);  /*chocolate*/
  	s2scr(35, 0.678431372549020,1,0.184313725490196);  /*green yellow*/
  	
   pushVRMLname("P1_line_Stokes");
   plotPnstab_new_Pts(P1,18,2,3,0.1,0.2);
   pushVRMLname("P2_line_Stokes");
   plotPnstab_new_Pts(P2,18,2,3,0.1,0.2); 
   pushVRMLname("P1_pnts_Re1");
   plotPnstab_new_Pts(P1re1,18,2,3,0.4,0.4);
   
//   s2sci(6);
//   s2pt(ptscuboid_for.ns,ptscuboid_for.xs,ptscuboid_for.ys,ptscuboid_for.zs,1);
   
//   double nms=4752.0;   
     int npart;
   npart=1089;
   
    s2sci(7);
   pushVRMLname("p1pt518_initial_full_square");
   struct Pts3d pts518b_1=ReadSelectedPts3d(pts518b, 0, npart*1);
   s2pt(pts518b_1.ns,pts518b_1.xs,pts518b_1.ys,pts518b_1.zs,1);
   s2sci(6);
   pushVRMLname("p1pt518_backw_prds_1_13");
   struct Pts3d pts518b_2=ReadSelectedPts3d(pts518b, npart*1, npart*14);
   s2pt(pts518b_2.ns,pts518b_2.xs,pts518b_2.ys,pts518b_2.zs,1);
   
   pushVRMLname("p1pt518_backw_prds_just_prd_300");
   struct Pts3d pts518b_3=ReadSelectedPts3d(pts518b, npart*300, npart*301);
   s2pt(pts518b_3.ns,pts518b_3.xs,pts518b_3.ys,pts518b_3.zs,1);
   
   pushVRMLname("p1pt518_backw_prds_just_prd_600");
   struct Pts3d pts518b_4=ReadSelectedPts3d(pts518b, npart*600, npart*601);
   s2pt(pts518b_4.ns,pts518b_4.xs,pts518b_4.ys,pts518b_4.zs,1);
   
   s2sci(1);
   pushVRMLname("p1pt518_forw_prds_1_13");
   struct Pts3d pts518f_2=ReadSelectedPts3d(pts518f, npart*1, npart*14);
   s2pt(pts518f_2.ns,pts518f_2.xs,pts518f_2.ys,pts518f_2.zs,1);
   
   pushVRMLname("p1pt518_forw_prds_just_prd_300");
   struct Pts3d pts518f_3=ReadSelectedPts3d(pts518f, npart*300, npart*301);
   s2pt(pts518f_3.ns,pts518f_3.xs,pts518f_3.ys,pts518f_3.zs,1);
   
   pushVRMLname("p1pt518_forw_prds_just_prd_600");
   struct Pts3d pts518f_4=ReadSelectedPts3d(pts518f, npart*600, npart*601);
   s2pt(pts518f_4.ns,pts518f_4.xs,pts518f_4.ys,pts518f_4.zs,1); 
   
   s2sci(7);
   pushVRMLname("p1pt513_initial_full_square");
   struct Pts3d pts513b_1=ReadSelectedPts3d(pts513b, 0, npart*1);
   s2pt(pts513b_1.ns,pts513b_1.xs,pts513b_1.ys,pts513b_1.zs,1);
   s2sci(6);
   pushVRMLname("p1pt513_backw_prds_1_16");
   struct Pts3d pts513b_2=ReadSelectedPts3d(pts513b, npart*1, npart*17);
   s2pt(pts513b_2.ns,pts513b_2.xs,pts513b_2.ys,pts513b_2.zs,1);
   
   pushVRMLname("p1pt513_backw_prds_just_prd_300");
   struct Pts3d pts513b_3=ReadSelectedPts3d(pts513b, npart*300, npart*301);
   s2pt(pts513b_3.ns,pts513b_3.xs,pts513b_3.ys,pts513b_3.zs,1);
   
   pushVRMLname("p1pt513_backw_prds_just_prd_600");
   struct Pts3d pts513b_4=ReadSelectedPts3d(pts513b, npart*600, npart*601);
   s2pt(pts513b_4.ns,pts513b_4.xs,pts513b_4.ys,pts513b_4.zs,1);
   
   s2sci(1);
   pushVRMLname("p1pt513_forw_prds_1_16");
   struct Pts3d pts513f_2=ReadSelectedPts3d(pts513f, npart*1, npart*17);
   s2pt(pts513f_2.ns,pts513f_2.xs,pts513f_2.ys,pts513f_2.zs,1);
   
   pushVRMLname("p1pt513_forw_prds_just_prd_300");
   struct Pts3d pts513f_3=ReadSelectedPts3d(pts513f, npart*300, npart*301);
   s2pt(pts513f_3.ns,pts513f_3.xs,pts513f_3.ys,pts513f_3.zs,1);
   
   pushVRMLname("p1pt513_forw_prds_just_prd_600");
   struct Pts3d pts513f_4=ReadSelectedPts3d(pts513f, npart*600, npart*601);
   s2pt(pts513f_4.ns,pts513f_4.xs,pts513f_4.ys,pts513f_4.zs,1);  
   
   s2sci(7);
   pushVRMLname("p1pt508_initial_full_square");
   struct Pts3d pts508b_1=ReadSelectedPts3d(pts508b, 0, npart*1);
   s2pt(pts508b_1.ns,pts508b_1.xs,pts508b_1.ys,pts508b_1.zs,1);
   s2sci(6);
   pushVRMLname("p1pt508_backw_prds_just_prd_19");
   struct Pts3d pts508b_2=ReadSelectedPts3d(pts508b, npart*19, npart*20);
   s2pt(pts508b_2.ns,pts508b_2.xs,pts508b_2.ys,pts508b_2.zs,1);
   
   pushVRMLname("p1pt508_backw_prds_just_prd_300");
   struct Pts3d pts508b_3=ReadSelectedPts3d(pts508b, npart*300, npart*301);
   s2pt(pts508b_3.ns,pts508b_3.xs,pts508b_3.ys,pts508b_3.zs,1);
   
   pushVRMLname("p1pt508_backw_prds_just_prd_600");
   struct Pts3d pts508b_4=ReadSelectedPts3d(pts508b, npart*600, npart*601);
   s2pt(pts508b_4.ns,pts508b_4.xs,pts508b_4.ys,pts508b_4.zs,1); 
   
//   pushVRMLname("p1pt508_backw_prds_just_point");
//   s2pt(pts508b_s.ns,pts508b_s.xs,pts508b_s.ys,pts508b_s.zs,1);
   s2sci(1);
   pushVRMLname("p1pt508_forw_prds_just_prd_19");
   struct Pts3d pts508f_2=ReadSelectedPts3d(pts508f, npart*19, npart*20);
   s2pt(pts508f_2.ns,pts508f_2.xs,pts508f_2.ys,pts508f_2.zs,1);
   
   pushVRMLname("p1pt508_forw_prds_just_prd_300");
   struct Pts3d pts508f_3=ReadSelectedPts3d(pts508f, npart*300, npart*301);
   s2pt(pts508f_3.ns,pts508f_3.xs,pts508f_3.ys,pts508f_3.zs,1);
   
   pushVRMLname("p1pt508_forw_prds_just_prd_600");
   struct Pts3d pts508f_4=ReadSelectedPts3d(pts508f, npart*600, npart*601);
   s2pt(pts508f_4.ns,pts508f_4.xs,pts508f_4.ys,pts508f_4.zs,1);  
   
   //pushVRMLname("p1pt508_forw_prds_just_point");
   //s2pt(pts508f_s.ns,pts508f_s.xs,pts508f_s.ys,pts508f_s.zs,1);
   
   pushVRMLname("P2_pnt_track_Re1");
   s2sch(0.4);
   s2sci(32);
   s2pt1(p2ptorb.xs[0], p2ptorb.ys[0], p2ptorb.zs[0], 4);
   s2sci(32);
   s2pt(p2ptorb.ns,p2ptorb.xs,p2ptorb.ys,p2ptorb.zs,1); 
   
   //pushVRMLname("P1pt2_mani_orbit_odd_prds_1_5000");
  
   s2sci(35);
   pushVRMLname("mani_orbit_ini_pt");
   s2pt1(p1pt2orb_odd.xs[0], p1pt2orb_odd.ys[0], p1pt2orb_odd.zs[0], 4);
   pushVRMLname("mani_orbit_odd_1_5000");
   struct Pts3d p1pt2orb_odd1=ReadSelectedPts3d(p1pt2orb_odd, 1, 2500);
   s2pt(p1pt2orb_odd1.ns,p1pt2orb_odd1.xs,p1pt2orb_odd1.ys,p1pt2orb_odd1.zs,1); 
   pushVRMLname("mani_orbit_odd_5001_9000");
   struct Pts3d p1pt2orb_odd2=ReadSelectedPts3d(p1pt2orb_odd, 2500, 4500);
   s2pt(p1pt2orb_odd2.ns,p1pt2orb_odd2.xs,p1pt2orb_odd2.ys,p1pt2orb_odd2.zs,1); 
   pushVRMLname("mani_orbit_odd_9001_15000");
   struct Pts3d p1pt2orb_odd3=ReadSelectedPts3d(p1pt2orb_odd, 4500, 7500);
   s2pt(p1pt2orb_odd3.ns,p1pt2orb_odd3.xs,p1pt2orb_odd3.ys,p1pt2orb_odd3.zs,1); 
   
   pushVRMLname("mani_orbit_even_1_5000");
   s2sci(8);
   //pushVRMLname("P1pt2_mani_orbit_even_prds_1_5000");
   struct Pts3d p1pt2orb_even1=ReadSelectedPts3d(p1pt2orb_even, 1, 2500);
   s2pt(p1pt2orb_even1.ns,p1pt2orb_even1.xs,p1pt2orb_even1.ys,p1pt2orb_even1.zs,1); 
   pushVRMLname("mani_orbit_even_5001_9000");
   struct Pts3d p1pt2orb_even2=ReadSelectedPts3d(p1pt2orb_even, 2500, 4500);
   s2pt(p1pt2orb_even2.ns,p1pt2orb_even2.xs,p1pt2orb_even2.ys,p1pt2orb_even2.zs,1); 
   pushVRMLname("mani_orbit_even_9001_15000");
   struct Pts3d p1pt2orb_even3=ReadSelectedPts3d(p1pt2orb_even, 4500, 7500);
   s2pt(p1pt2orb_even3.ns,p1pt2orb_even3.xs,p1pt2orb_even3.ys,p1pt2orb_even3.zs,1); 
   

   s2sci(35);
   pushVRMLname("p1pt513_near_ini_pt");
   s2pt1(pts513f_s.xs[0], pts513f_s.ys[0], pts513f_s.zs[0], 4);
   pushVRMLname("p1pt513_near_Fodd_1_80");
   struct Pts3d pts513f_s_odd1=ReadSelectedPts3d_odd(pts513f_s, 0, 80);
   s2pt(pts513f_s_odd1.ns,pts513f_s_odd1.xs,pts513f_s_odd1.ys,pts513f_s_odd1.zs,1); 
   pushVRMLname("p1pt513_near_Fodd_81_150");
   struct Pts3d pts513f_s_odd2=ReadSelectedPts3d_odd(pts513f_s, 80, 150);
   s2pt(pts513f_s_odd2.ns,pts513f_s_odd2.xs,pts513f_s_odd2.ys,pts513f_s_odd2.zs,1); 
   pushVRMLname("p1pt513_near_Fodd_151_1500");
   struct Pts3d pts513f_s_odd3=ReadSelectedPts3d_odd(pts513f_s, 150, 1500);
   s2pt(pts513f_s_odd3.ns,pts513f_s_odd3.xs,pts513f_s_odd3.ys,pts513f_s_odd3.zs,1); 
   pushVRMLname("p1pt513_near_Fodd_1501_2000");
   struct Pts3d pts513f_s_odd5=ReadSelectedPts3d_odd(pts513f_s, 1500, 2000);
   s2pt(pts513f_s_odd5.ns,pts513f_s_odd5.xs,pts513f_s_odd5.ys,pts513f_s_odd5.zs,1);    
   pushVRMLname("p1pt513_near_Fodd_2001_5600");
   struct Pts3d pts513f_s_odd4=ReadSelectedPts3d_odd(pts513f_s, 2000, 5600);
   s2pt(pts513f_s_odd4.ns,pts513f_s_odd4.xs,pts513f_s_odd4.ys,pts513f_s_odd4.zs,1); 
   s2sci(8);
   pushVRMLname("p1pt513_near_Feven_1_80");
   struct Pts3d pts513f_s_even1=ReadSelectedPts3d_even(pts513f_s, 0, 80);
   s2pt(pts513f_s_even1.ns,pts513f_s_even1.xs,pts513f_s_even1.ys,pts513f_s_even1.zs,1); 
   pushVRMLname("p1pt513_near_Feven_81_150");
   struct Pts3d pts513f_s_even2=ReadSelectedPts3d_even(pts513f_s, 80, 150);
   s2pt(pts513f_s_even2.ns,pts513f_s_even2.xs,pts513f_s_even2.ys,pts513f_s_even2.zs,1); 
   pushVRMLname("p1pt513_near_Feven_151_1500");
   struct Pts3d pts513f_s_even3=ReadSelectedPts3d_even(pts513f_s, 150, 1500);
   s2pt(pts513f_s_even3.ns,pts513f_s_even3.xs,pts513f_s_even3.ys,pts513f_s_even3.zs,1); 
   pushVRMLname("p1pt513_near_Feven_1501_2000");
   struct Pts3d pts513f_s_even5=ReadSelectedPts3d_even(pts513f_s, 1500, 2000);
   s2pt(pts513f_s_even5.ns,pts513f_s_even5.xs,pts513f_s_even5.ys,pts513f_s_even5.zs,1); 
   
   pushVRMLname("p1pt513_near_Feven_2001_5600");
   struct Pts3d pts513f_s_even4=ReadSelectedPts3d_even(pts513f_s, 2000, 5600);
   s2pt(pts513f_s_even4.ns,pts513f_s_even4.xs,pts513f_s_even4.ys,pts513f_s_even4.zs,1); 
   
   s2sci(35);
   pushVRMLname("p1pt513_near_Bodd_1_5600");
   struct Pts3d pts513b_s_odd1=ReadSelectedPts3d_odd(pts513b_s, 0, 5600);
   s2pt(pts513b_s_odd1.ns,pts513b_s_odd1.xs,pts513b_s_odd1.ys,pts513b_s_odd1.zs,1); 
   s2sci(8);
   pushVRMLname("p1pt513_near_Beven_1_5600");
   struct Pts3d pts513b_s_even1=ReadSelectedPts3d_even(pts513b_s, 0, 5600);
   s2pt(pts513b_s_even1.ns,pts513b_s_even1.xs,pts513b_s_even1.ys,pts513b_s_even1.zs,1); 
   
   
/*   //   struct Pts3d ptsprtf5=ReadSelectedPts3d(ptscuboid_for, 5*4752, 6*4752);
   s2pt(ptsprtf5.ns,ptsprtf5.xs,ptsprtf5.ys,ptsprtf5.zs,1);
   
   pushVRMLname("cuboid_forw_10prds");
//   struct Pts3d ptsprtf10=ReadSelectedPts3d(ptscuboid_for, 10*nms, 11*nms);
   s2pt(ptsprtf10.ns,ptsprtf10.xs,ptsprtf10.ys,ptsprtf10.zs,1);
   
   pushVRMLname("cuboid_forw_15prds");
//   struct Pts3d ptsprtf15=ReadSelectedPts3d(ptscuboid_for, 15*nms, 16*nms);
   s2pt(ptsprtf15.ns,ptsprtf15.xs,ptsprtf15.ys,ptsprtf15.zs,1);
   
   pushVRMLname("cuboid_forw_20prds");
//   struct Pts3d ptsprtf20=ReadSelectedPts3d(ptscuboid_for, 20*nms, 21*nms);
   s2pt(ptsprtf20.ns,ptsprtf20.xs,ptsprtf20.ys,ptsprtf20.zs,1);
   
   s2sci(6);
   pushVRMLname("cuboid_back_5prds");
//   struct Pts3d ptsprtb5=ReadSelectedPts3d(ptscuboid_back, 5*nms, 6*nms);
   s2pt(ptsprtb5.ns,ptsprtb5.xs,ptsprtb5.ys,ptsprtb5.zs,1);
   
   pushVRMLname("cuboid_back_10prds");
//   struct Pts3d ptsprtb10=ReadSelectedPts3d(ptscuboid_back, 10*nms, 11*nms);
   s2pt(ptsprtb10.ns,ptsprtb10.xs,ptsprtb10.ys,ptsprtb10.zs,1);
   
   pushVRMLname("cuboid_back_15prds");
 //  struct Pts3d ptsprtb15=ReadSelectedPts3d(ptscuboid_back, 15*nms, 16*nms);
   s2pt(ptsprtb15.ns,ptsprtb15.xs,ptsprtb15.ys,ptsprtb15.zs,1);
   
   pushVRMLname("cuboid_back_20prds");
//   struct Pts3d ptsprtb20=ReadSelectedPts3d(ptscuboid_back, 20*nms, 21*nms);
   s2pt(ptsprtb20.ns,ptsprtb20.xs,ptsprtb20.ys,ptsprtb20.zs,1);  */
   
   
   s2show(1);					/* Open the s2plot window */

   
   return 1;
}
