-- bitwise.lua

function BitXOR(a,b)--Bitwise xor
local p,c=1,0
while a>0 and b>0 do
local ra,rb=a%2,b%2
if ra~=rb then c=c+p end
a,b,p=(a-ra)/2,(b-rb)/2,p*2
end
if a<b then a=b end
while a>0 do
local ra=a%2
if ra>0 then c=c+p end
a,p=(a-ra)/2,p*2
end
return c
end

function BitOR(a,b)--Bitwise or
local p,c=1,0
while a+b>0 do
local ra,rb=a%2,b%2
if ra+rb>0 then c=c+p end
a,b,p=(a-ra)/2,(b-rb)/2,p*2
end
return c
end

function BitNOT(n)
local p,c=1,0
while n>0 do
local r=n%2
if r<1 then c=c+p end
n,p=(n-r)/2,p*2
end
return c
end

function BitAND(a,b)--Bitwise and
local p,c=1,0
while a>0 and b>0 do
local ra,rb=a%2,b%2
if ra+rb>1 then c=c+p end
a,b,p=(a-ra)/2,(b-rb)/2,p*2
end
return c
end