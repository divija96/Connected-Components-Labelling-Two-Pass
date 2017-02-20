#include <stdio.h>
#include <time.h>
void flatten(int p[100],int count){
	int k=1,i,j;
	for(i=1;i<=count;i++){
		if(p[i]<i){
			p[i]=p[p[i]];
		}
		else{
			p[i]=k;
			k++;
		}
	}
}

int merge(int p[100],int x,int y){
	int rootx=x,rooty=y,z;
	while(p[rootx]!=p[rooty]){
		if(p[rootx]>p[rooty]){
			if(p[rootx]==rootx){
				p[rootx]=p[rooty];
				return p[rootx];
			}
			z=p[rootx];
			p[rootx]=p[rooty];
			rootx=z;
		}
		else{
			if(rooty==p[rooty]){
				p[rooty]=p[rootx];
				return p[rootx];
			}
			z=p[rooty];
			p[rooty]=p[rootx];
			rooty=z;
		}
	}
	return (p[rootx]);
}

int scan(int matrix[100][100],int label[100][100],int p[100],int m,int n,int count){
	int ax,ay,bx,by,cx,cy,dx,dy,fx,fy,gx,gy,i,j;
    for(i=1;i<m+1;i=i+2){
    	for(j=1;j<n+1;j++){
    	    ax=i-1;ay=j-1;bx=i-1;by=j;cx=i-1;cy=j+1;dx=i;dy=j-1;fx=i+1;fy=j-1;gx=i+1;gy=j;
    	    if(matrix[i][j]==1){
    	  		if(matrix[dx][dy]==0){
    	  			if(matrix[bx][by]==1){
    	  				label[i][j]=label[bx][by];
    	  				if(matrix[fx][fy]){
    	  					merge(p,label[i][j],label[fx][fy]);
    	  				}
    	  			}
    	  			else{
    	  				if(matrix[fx][fy]==1){
    	  					label[i][j]=label[fx][fy];
    	  					if(matrix[ax][ay]==1){
    	  						merge(p,label[i][j],label[ax][ay]);
    	  					}
    	  					if(matrix[cx][cy]==1){
    	  						merge(p,label[i][j],label[cx][cy]);
    	  					}
    	  				}
    	  				else{
    	  					if(matrix[ax][ay]==1){
    	  						label[i][j]=label[ax][ay];
    	  						if(matrix[cx][cy]==1){
    	  							merge(p,label[i][j],label[cx][cy]);
    	  						}
    	  					}
    	  					else{
    	  						if(matrix[cx][cy]==1){
    	  							label[i][j]=label[cx][cy];
    	  						}
    	  						else{
    	  							label[i][j]=count;
    	  							p[count]=count;
    	  							count++;
    	  						}
    	  					}
    	  				}
    	  			}
    	  		}
    	  		else{
    	  			label[i][j]=label[dx][dy];
    	  			if(matrix[bx][by]==0){
    	  				if(matrix[cx][cy]==1){
    	  					merge(p,label[i][j],label[cx][cy]);
    	  				}
    	  			}
    	  		}
    	  		if(matrix[gx][gy]==1){
    	  			label[gx][gy]=label[i][j];
    	  		}
    	  	}
    	  	else{
    	  		if(matrix[gx][gy]==1){
    	  			if(matrix[dx][dy]==1){
    	  				label[gx][gy]=label[dx][dy];
    	  			}
    				else{
    					if(matrix[fx][fy]==1){
    						label[gx][gy]=label[fx][fy];
    					}
    					else{
    						label[gx][gy]=count;
    						p[count]=count;
    						count++;
    					}
    				}
    	  		}
    	  	}
    	}
    }
    return(count);
}

void twopass(int matrix[100][100],int m,int n){
	double time_spent,t1,t2;
  struct timespec tp,tp1;
  clock_gettime(CLOCK_REALTIME,&tp);
  t1 = (((double)tp.tv_sec) * 1000000) + (((double)tp.tv_nsec) / 1000) ;
	int label[100][100],count=1,p[100]={0},i,j;
	count=scan(matrix,label,p,m,n,count);
	flatten(p,count);
	for(i=1;i<m+1;i++){
		for(j=1;j<n+1;j++){
			label[i][j]=p[label[i][j]];
		}
	}
	printf("The labelled components are : \n");
	for(i=1;i<m+1;i++){
		for(j=1;j<n+1;j++){
			printf("%d ",label[i][j]);
		}
		printf("\n");
	}
	clock_gettime(CLOCK_REALTIME,&tp1);
  t2 = (((double)tp1.tv_sec) * 1000000) + (((double)tp1.tv_nsec) / 1000) ;
  time_spent = t2 -t1;
  printf("Time spent : %f\n",time_spent);
}
void main(){
	int m,n,i,j,matrix[100][100];
	printf("enter rows and columns");
	scanf("%d%d",&m,&n);
	for(i=0;i<m+2;i++){
		for(j=0;j<n+2;j++){
			if(i==0||j==0||i==m+1||j==n+1){
				matrix[i][j]=0;
			}
			else{
				scanf("%d",&matrix[i][j]);
			}
		}
	}
	twopass(matrix,m,n);
}
