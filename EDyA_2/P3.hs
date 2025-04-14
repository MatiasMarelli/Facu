module Lab01 where
  
import Data.List
import Data.Char
type Color = (Int,Int,Int)
-- data Color = RGB Int Int Int
{-
  data Color = RGB{
                red :: Int
                green :: Int
                blue :: Int
              }

-}
--mezcla :: Color -> Color -> Color
--mezcla (r1,g1,b1) (r2,g2,b2) = (div (r1 + r2) 2 , div (g1 + g2) 2 , div (b1 + b2) 2)
--        where avg x y = div (x + y) 2
--        (avg r1 r2, avg g1 g2,avg b1 b2)
--  4)
data Exp = Lit Int | Add Exp Exp | Sub Exp Exp | Prod Exp Exp | Div Exp Exp

eval :: Exp -> Int

eval (Lit n) = n
eval (Add e1 e2) = eval e1 + eval e2
eval (Sub e1 e2) = eval e1 - eval e2
eval (Prod e1 e2) = eval e1 * eval e2
eval (Div e1 e2) = div (eval e1) (eval e2)

--  5)                      

getFirstA :: String->String->(String,String)
getFirstA "" x = (x,"")
getFirstA (s:ss) x | ss == "" = (x',ss)
                   | s == ' ' = (x,ss)
                   | otherwise = getFirstA ss x'
                   where x' = (reverse(s:reverse(x)))

getFirst :: String->(String,String)
getFirst s = getFirstA s ""

parseS :: [Exp] -> String -> Exp
parseS p "" = if(length p == 1)
               then head p
               else error "EMF"

parseS p s = case x of
  "+" -> parseS ((Add t' t):p') s'
  "-" -> parseS ((Sub t' t):p') s'
  "*" -> parseS ((Prod t' t):p') s'
  "/" -> parseS ((Div t' t):p') s'
  n -> parseS ((Lit (read n)):p) s'
  where (x,s') = getFirst s
        t = head(p)
        t' = head(tail(p))
        p' = tail(tail(p))

parseRPN :: String -> Exp
parseRPN s = parseS [] s

solRPN :: String->Int
solRPN s = eval(parseRPN s)