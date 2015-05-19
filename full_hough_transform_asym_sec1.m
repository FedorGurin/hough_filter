Упрощенный вариант на матлаб.
 
function [A,index]=full_hough_transform_asym_sec1(x,R0,sec)
 
%R - степень 2, номер сектора от 1 до 8
%Вариант для реализации на С++, используется один буфер
%Матричные операции реализуются функциями ippiAdd (IPP) или nppiAdd (CUDA)
 
[N,M]=size(x);
 
 
A=zeros(N,M,R0);
A(:,:,1)=x;
R=1;
CFS=R0/2;
 
 
while R<R0
    [index,di]=getindex(R,sec);
 
    for r=1:R
  % Левый верхний угол   источника и приемника
 
    v1=(abs(index(1,r))-index(1,r))/2;
   v2=(abs(index(1,r))+index(1,r))/2;
   u1=(abs(index(2,r))-index(2,r))/2;
   u2=(abs(index(2,r))+index(2,r))/2;
   v3=v1+(abs(di(1,r))-di(1,r))/2;
   v4=v2+(abs(di(1,r))+di(1,r))/2;
   u3=u1+(abs(di(2,r))-di(2,r))/2;
   u4=u2+(abs(di(2,r))+di(2,r))/2;
  
%индекс слоя
   rr=CFS*(r-1)*2+1;
  
      A(v3+1:v3+N-R,u3+1:u3+M-R,rr+CFS)=A(v3+1:v3+N-R,u3+1:u3+M-R,rr)+A(v4+1:v4+N-R,u4+1:u4+M-R,rr);
    A(v1+1:v1+N-R,u1+1:u1+M-R,rr)=A(v1+1:v1+N-R,u1+1:u1+M-R,rr)+A(v2+1:v2+N-R,u2+1:u2+M-R,rr);
  
   end
   
  R=2*R;CFS=CFS/2;
end
 
index=getindex(R,sec);
     
 
    function [index,di]=getindex(R,sec)
      index0=[R:-1:-R  -repmat(R,[1 2*R-1])  -R:R repmat(R,[1 2*R-1]);  repmat(R,[1 2*R]) R:-1:-R  repmat(-R,[1 2*R-1]) -R:R-1 ];
  di0=[diff(index0(1,[1:end 1])); diff(index0(2,[1:end 1])) ];
  index=index0(:,(sec-1)*R+(1:R));
  di=di0(:,(sec-1)*R+(1:R));


    