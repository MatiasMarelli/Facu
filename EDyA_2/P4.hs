data Tree a = E | Node (Tree a) a (Tree a) deriving Show

completo :: a -> Int -> Tree a
completo x 0 = (Node E x E)
completo x d = let z = (completo x (d-1)) in (Node z x z)

balanceado :: a -> Int -> Tree a
balanceado _ 0 = E
balanceado x n |mod 2 n == 0 = let t = div (n-1) 2 in Node (balanceado x t) x (balanceado x (n-1-t))  
               |otherwise = let b = balanceado x (div (n-1) 2) in (Node b x b)



type BST  = Tree 

maximum' :: Ord a => BST a -> a
maximum' E = error "Arbol Vacio !!!"
maximum' (Node E a E) = a
maximum' (Node _ _ r) = maximum' r 

minimum' :: Ord a => BST a -> a
minimum' E = error "Arbol Vacio !!!"
minimum' (Node E a E) = a
minimum' (Node l _ _) = minimum' l 



checkBST :: Ord a => BST a -> Bool
checkBST E = True
checkBST (Node E a E) = True
checkBST (Node l a r)= ((a < (minimum' r)) && (a >= (maximum' l)) && (checkBST l) && (checkBST r))


splitBST :: Ord a => BST a -> a -> (BST a,BST a)

splitBST E _ = (E,E)
splitBST (Node l y r) x | x == y = (Node l y E,r)
                       | x < y = let (l',r') = splitBST l x in (l' , Node r' y r)
                       | otherwise = let (l',r')= splitBST r x in (Node l y l',r')

insertBST :: Ord a => BST a -> a -> BST a

insertBST E x = Node E x E
insertBST (Node l y r) x | x <= y = Node (insertBST l x) y r
                         | otherwise = Node l y (insertBST r x)  

joinBST :: Ord a => BST a -> BST a -> BST a

joinBST t E = t
joinBST E t = t
joinBST (Node l x r) t = let (l',r') = splitBST t x in Node (joinBST l l') x (joinBST r r') 

-------------------------------------------------------------------------------------------------------------------------------------------

data Color = R | B
data RBT a = Empty | T Color (RBT a) a (RBT a)

member :: Ord a => a -> RBT a ->Bool

member _ Empty = False 
member x (T _ l y r) |x == y =True
                     |x < y = member x l
                     |otherwise = member x r


balance ::  Color -> RBT a -> a -> RBT a -> RBT a

balance B (T R (T R a x b) y c) z d = T R (T B a x b) y (T B c z d)
balance B (T R a x (T R b y c)) z d = T R (T B a x b) y (T B c z d)
balance B a x (T R (T R b y c) z d) = T R (T B a x b) y (T B c z d)
balance B a x (T R b y (T R c z d)) = T R (T B a x b) y (T B c z d)
balance c l a r = T c l a r

ins :: Ord a => a -> RBT a -> RBT a

ins x Empty = T R Empty x Empty
ins x (T c l y r) | x < y = balance c (ins x l) y r
                  | x > y = balance c l y (ins x r)
                  | otherwise = T c l y r 

makeBlack :: RBT a -> RBT a

makeBlack Empty = Empty
makeBlack (T _ l x r) = (T B l x r)
  

insertRBT :: Ord a => a -> RBT a -> RBT a
insertRBT x t = makeBlack(ins x t)


balance_l ::  Color -> RBT a -> a -> RBT a -> RBT a

balance_l B (T R (T R a x b) y c) z d = T R (T B a x b) y (T B c z d)
balance_l B (T R a x (T R b y c)) z d = T R (T B a x b) y (T B c z d)
balance_l c l a r = T c l a r

balance_r ::  Color -> RBT a -> a -> RBT a -> RBT a

balance_r B a x (T R (T R b y c) z d) = T R (T B a x b) y (T B c z d)
balance_r B a x (T R b y (T R c z d)) = T R (T B a x b) y (T B c z d)
balance_r c l a r = T c l a r

insert2 :: Ord a => a -> RBT a -> RBT a

insert2 x t = makeBlack(ins2 x t)
  where ins2 x Empty = T R Empty x Empty
        ins2 x (T c l y r) | x < y     = balance_l c (ins x l) y r
                           | x > y     = balance_r c l y (ins x r)
                           | otherwise = T c l y r 

--      VAMOS * 2

data Tree123 a = E123 | T2 a (Tree123 a) (Tree123 a) | T3 a a (Tree123 a) (Tree123 a) (Tree123 a) | T4 a a a (Tree123 a) (Tree123 a) (Tree123 a) (Tree123 a)

rbtTO123 :: RBT a -> Tree123 a 

rbtTO123 Empty = E123
rbtTO123 (T B (T R ll x rl) y (T R lr z rr)) = T4 x y z (rbtTO123 ll) (rbtTO123 rl) (rbtTO123 lr) (rbtTO123 rr)
rbtTO123 (T B (T R l x r) y tr) = T3 x y (rbtTO123 l) (rbtTO123 r) (rbtTO123 tr)
rbtTO123 (T B tl x (T R l y r)) = T3 x y (rbtTO123 tl) (rbtTO123 l) (rbtTO123 r)
rbtTO123 (T B l x r) = T2 x (rbtTO123 l ) (rbtTO123 r) 
rbtTO123 (T R l x r) = rbtTO123(T B l x r)

--------------------------------------------------------------------------------------------------------------------------------------------------------------

type Rank = Int
data Heap a = Emp | N Rank a (Heap a) (Heap a)

rank :: Heap a -> Rank

rank Emp         = 0
rank (N r _ _ _) = r

makeH :: Ord a => a -> Heap a -> Heap a -> Heap a

makeH x h1 h2 = if rank h1 >= rank h2 then N (rank h2 + 1) x h1 h2 
                                      else N (rank h1 + 1) x h2 h1 

merge :: Ord a => Heap a -> Heap a -> Heap a 

merge h1 Emp = h1
merge Emp h2 = h2
merge h1@(N _  x l1 r1) h2@(N _  y l2 r2) = if x <= y then makeH x l1 (merge r1 h2)
                                                      else makeH y l2 (merge r2 h1)

insertH :: Ord a => a -> Heap a -> Heap a 
insertH a h = merge (N 1 a Emp Emp) h

minH :: Ord a => Heap a -> a
minH (N _ x a b) = x 

deleteMin :: Ord a => Heap a -> Heap a 
deleteMin (N _ _ a b) = merge a b

fromList :: Ord a => [a] -> Heap a
fromList [] = Emp
fromList (x:xs) = merge (N 1 x Emp Emp) (fromList xs) 

data PHeaps a = EPH | Root a [PHeaps a ]


isPHeap :: Ord a => PHeaps a -> Bool

isPHeap EPH = True
isPHeap (Root a []) = True
isPHeap (Root a ( ph@(Root v _):xs)) = a<v &&  isPHeap ph && isPHeap (Root a xs)

mergePH :: Ord a => PHeaps a -> PHeaps a ->PHeaps a

mergePH EPH h = h
mergePH h EPH = h
mergePH h1@(Root x (xh:xhs)) h2@(Root y (yh:yhs)) = if x<= y then Root y (h1:(yh:yhs))
                                                             else Root x (h2:(xh:xhs))

insertPH :: Ord a => PHeaps a -> a -> PHeaps a

insertPH h x = mergePH h (Root x [])

concatHeaps :: Ord a => [PHeaps a] -> PHeaps a 
concatHeaps [] = EPH
concatHeaps (h:hs) = mergePH h (concatHeaps hs)

delMin :: Ord a => PHeaps a -> Maybe(a,PHeaps a)

delMin EPH = Nothing
delMin (Root x h) = Just(x,concatHeaps h)