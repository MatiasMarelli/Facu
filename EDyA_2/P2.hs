

-- Ejercicio 5--

ptoA :: a->a
ptoA x = let (y,z) = (x,x) in y

ptoA' :: a->a
ptoA' x = x

ptoB :: Ord a => (a,a)->Bool
ptoB (x,y) = if x > y then True else False

ptoB' :: Ord a => (a,a)->Bool
ptoB' (x,y) = x > y

ptoC :: Num a => (a,a)->a
ptoC (x,y) = let z = x + y in g (z,y) where g (a,b) = a - b

ptoC' :: Num a => (a,a)->a
ptoC' (x,y) = x

-- Ejercicio 9 --

zipThree :: [a]->[b]->[c]->[(a,b,c)]
zipThree [] _ _ = []
zipThree _ [] _ = []
zipThree _ _ [] = []
zipThree (x:xs) (y:ys) (z:zs) = ((x,y,z):zipThree xs ys zs)

-- Ejercicio 12 --

-- Definicion de tipos --
type Bit = Bool
type NumBin = [Bit]
type Resto = Bit

-- xor de bits --
xor :: Bit->Bit->Bit
xor x y = x /= y

-- Suma de bits, retorna suma y carry --
sumaBit :: (Bit,Bit)->(Bit,Bit)
sumaBit (x,y) = if((x == True) && (y == True)) then (False,True) else (xor x y,False)

-- Suma dos numeros binarios con carry recursivamente --
sumaBinC :: (NumBin,NumBin,Bit)->NumBin
sumaBinC ([],[],carry) = if(carry) then [carry] else []
sumaBinC ((x:xs),[],carry) = let z = sumaBit(x,carry) in fst(z):sumaBinC([],xs,snd(z))
sumaBinC ([],(y:ys),carry) = let z = sumaBit(y,carry) in fst(z):sumaBinC([],ys,snd(z))
sumaBinC ((x:xs),(y:ys),carry) = let w = sumaBit(x,y) 
                                     z = sumaBit(fst(w),carry) 
                                 in fst(z):sumaBinC(xs,ys,snd(w) || snd(z))

-- Suma dos numeros binarios --
sumaBin :: NumBin->NumBin->NumBin
sumaBin xs ys = sumaBinC(xs,ys,False)

-- Division por 2 de numero binario --
divPorDosBin :: NumBin->(NumBin,Resto)
divPorDosBin [] = ([],False)
divPorDosBin (x:xs) = (xs,x)

-- Ejercicio 13 --

-- (a) --

divisors :: Int -> [Int]
divisors x | x <= 0 = []
           | otherwise = [y | y <- [1..x], mod x y == 0]

-- (b) --

matches :: Int->[Int]->[Int]
matches x [] = []
matches x (y:ys) | x == y = x:matches x ys
                 | otherwise = matches x ys

-- (c) --

cuadruplas :: Int->[(Int,Int,Int,Int)]
cuadruplas n = [(a,b,c,d) | a <- [1..n], b <- [1..n], c <- [1..n], d <- [1..n], ((a^2) + (b^2)) == ((c^2) + (d^2))]

-- (d) --

unique :: [Int]->[Int]
unique [] = []
unique (x:xs) | matches x xs == [] = x:unique xs
              | otherwise = unique xs

-- Ejercicio 14 --

scalarProduct :: [Int]->[Int]->Int
scalarProduct xs ys = sum [a*b | (a,b) <- zip xs ys]