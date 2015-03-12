function y=full_hough_transform_sym(x,R0)

%R - степень 2

R0=R0/2;
[N,M]=size(x);

B=zeros(N,M,4*R0);
 
A=repmat(x,[1 1 4*R0]);

R=1;

v1=0:N-1;v2=0:M-1;


while R<R0
    
  index=[R:-1:-R  -repmat(R,[1 2*R-1]);  repmat(R,[1 2*R]) R:-1:-(R-1)];

   di=[diff(index(1,:)) 0; diff(index(2,:))  -1];

    for r=1:size(index,2)
    r1=2*r-1;r2=2*r;
    


    vv1=mod(v1+index(1,r),N);
    vv2=mod(v2+index(2,r),M);
     vv3=mod(vv1+di(1,r),N);
     vv4=mod(vv2+di(2,r),M);
     
    B(:,:,r1)=A(:,:,r)+A(vv1+1,vv2+1,r);
      B(:,:,r2)=A(:,:,r)+A(vv3+1,vv4+1,r);    
      
      
   end
    R=2*R;
      if (R==R0)
        y=sym(B);
      return;
      end
    
      
  index=[R:-1:-R  -repmat(R,[1 2*R-1]);  repmat(R,[1 2*R]) R:-1:-(R-1)];

di=[ diff(index(1,:)) 0; diff(index(2,:)) -1];


 for r=1:size(index,2)
    r1=2*r-1;r2=2*r;
     vv1=mod(v1+index(1,r),N);
    vv2=mod(v2+index(2,r),M);
     vv3=mod(vv1+di(1,r),N);
     vv4=mod(vv2+di(2,r),M);
     
    A(:,:,r1)=B(:,:,r)+B(vv1+1,vv2+1,r);
      A(:,:,r2)=B(:,:,r)+B(vv3+1,vv4+1,r);    
      
      
       end
  R=2*R;
  if (R==R0)
        y=sym(A);
      return;
      end
    
end


function  y=sym(A)
%Симметризация
[N,M,K]=size(A);
R=K/4;
 y=zeros(size(A));
  index=[R:-1:-R  -repmat(R,[1 2*R-1]);  repmat(R,[1 2*R]) R:-1:-(R-1)];
v1=0:N-1;v2=0:M-1;
for i=1:K
     vv1=mod(v1-index(1,i),N)+1;
    vv2=mod(v2-index(2,i),M)+1;
    y(:,:,i)=A(:,:,i)+A(vv1,vv2,i);
end
    
