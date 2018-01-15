close all;
clear;
clc;


if ~exist('s','var')
    %s=serial('dev/ttyUSB0,'BaudRate', 115200);
    s=serial('com3','BaudRate', 115200);
    
end

if strcmp(s.status,'closed')
    fopen(s);
end
pause(1)

z_max=5;

r=3;

x1=0;
y1=3;

x2=0;
y2=6;

x3=2.5;
y3=4.5;

xc0=1;
yc0=5;
zc0=2;

dz1Int = 1;
dz2Int = 1;
dz3Int = 1;

z1=zc0+sqrt(r^2-(x1-xc0)^2-(y1-yc0)^2);
z2=zc0+sqrt(r^2-(x2-xc0)^2-(y2-yc0)^2);
z3=zc0+sqrt(r^2-(x3-xc0)^2-(y3-yc0)^2);

col1=line([x1 x1], [y1 y1], [0 z_max],'Linewidth',6);
col2=line([x2 x2], [y2 y2], [0 z_max],'Linewidth',6);
col3=line([x3 x3], [y3 y3], [0 z_max],'Linewidth',6);

arm1=line([x1 xc0], [y1 yc0], [z1 zc0],'Linewidth',4,'Color',[1 0 0]);
arm2=line([x2 xc0], [y2 yc0], [z2 zc0],'Linewidth',4,'Color',[1 0 0]);
arm3=line([x3 xc0], [y3 yc0], [z3 zc0],'Linewidth',4,'Color',[1 0 0]);

view(14,4);



dzHome=-9999999;

tekst = ['x=' num2str(dzHome) ' y=' num2str(dzHome) ' z=' num2str(dzHome)];
fprintf(s,tekst);

while s.BytesAvailable()<1

end

while s.BytesAvailable()
    c=fscanf(s);
end

pause(4)

for t=0:3:100
    
    xc=0.1*sin(t) + xc0;
    yc=0.1*cos(t) + yc0;
    zc=zc0;
    
    z1=zc+sqrt(r^2-(x1-xc)^2-(y1-yc)^2);
    z2=zc+sqrt(r^2-(x2-xc)^2-(y2-yc)^2);
    z3=zc+sqrt(r^2-(x3-xc)^2-(y3-yc)^2);
    
    try
        dz1=z1-z1prev;
        dz2=z2-z2prev;
        dz3=z3-z3prev;
        
        dz1Int = int64(dz1*10000);
        dz2Int = int64(dz2*10000);
        dz3Int = int64(dz3*10000);
        
    catch err
        disp('Pierwsze odpalenie funkcji');
    end
        
    

    set(arm1, 'XData',[x1 xc], 'YData', [y1 yc], 'ZData', [z1 zc]);
    set(arm2, 'XData',[x2 xc], 'YData', [y2 yc], 'ZData', [z2 zc]);
    set(arm3, 'XData',[x3 xc], 'YData', [y3 yc], 'ZData', [z3 zc]);
    drawnow();
    
    z1prev=z1;
    z2prev=z2;
    z3prev=z3;
    
    
    

    
    tekst = ['x=' num2str(dz1Int) ' y=' num2str(dz2Int) ' z=' num2str(dz3Int)];
    fprintf(s,tekst);
    
    while s.BytesAvailable()<1
    end
       
    
    while s.BytesAvailable()
    c=fscanf(s);
    end
end
fclose(s);