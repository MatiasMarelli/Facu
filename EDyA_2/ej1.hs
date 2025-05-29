-- TestTree (Secuencia: 10 5 15 2 1 8) (Node 6 (Node 2 (Node 1 (Empty) 10 (Empty)) 5 (Empty)) 15 (Node 3 (Node 1 (Empty) 2 (Empty)) 1 (Node 1 (Empty) 8 (Empty))))

data BTree a = Empty | Node Int (BTree a) a (BTree a) deriving Show

(|||) :: a -> b -> (a,b)
x ||| y = (x,y)

size :: BTree a -> Int
size Empty = 0
size (Node n _ _ _) = n

nth :: BTree a -> Int -> a
nth Empty _ = error "INDEX ERROR"
nth (Node n l x r) i | i == sl = x
                     | i < sl = nth l i
                     | otherwise = nth r (i-sl-1)
                where sl = size l

cons :: a -> BTree a -> BTree a
cons a Empty = (Node 1 Empty a Empty)
cons a (Node n l x r) = (Node (n+1) (cons a l) x r)

tabulate :: (Int -> a) -> Int -> BTree a
tabulate f 0 = Node 1 Empty (f 0) Empty
tabulate f n = cons (f n) (tabulate f (n-1))



mapT :: (a -> b) -> BTree a -> BTree b
mapT _ Empty = Empty
mapT f (Node n l x r) = let ((l',r'), x') = (mapT f l ||| mapT f r) ||| (f x)
                        in (Node n l' x' r')


takeT :: Int -> BTree a -> BTree a

takeT _ Empty =error "INDEX ERROR"
takeT i (Node n l x r) | i == sl = Node (i+1) l x Empty
                      | i > sl = Node (i+1) l x r'
                      | otherwise = l'
                    where ((l',r'),sl) = ((takeT i l) ||| (takeT (i-(size l)-1) r)) ||| (size l) 


dropI :: Int -> BTree a -> BTree a

dropI _ Empty =error "INDEX ERROR"
dropI i (Node n l x r) |i == sl = r
                       |i > sl = r'
                       |otherwise = (Node (n-i-1) l' x r)
                       where ((l',r'),sl) = (((dropI i l) ||| (dropI (i-sl-1) r)) ||| (size l))

dropT :: Int -> BTree a -> BTree a

dropT _ Empty =error "INDEX ERROR"
dropT i (Node n l x r) | i==sl = Node (n-i) Empty x r
                       | i > sl = r'
                       | otherwise = (Node (n-i) l' x r)
                       where ((l',r'),sl)=(((dropT i l) ||| (dropT (i-sl-1) r)) |||(size l))

