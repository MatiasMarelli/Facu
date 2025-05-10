
data Color = R | B
data RBT a = Empty | T Color (RBT a) a (RBT a)

{- alt :: RBT a -> Int
alt Empty = 0
alt (T B l _ r) = 1 + alt l + alt r
alt (T R l _ r) = alt l + alt r

alturanegra :: RBT a -> Bool

alturanegra Empty =True
alturanegra (T c l _ r)= let (hizq,hder) = (alt l, alt r) in if hizq != hder then  

 -}

verificarAlturaNegra :: RBTree a -> Maybe Int
verificarAlturaNegra Vacio = Just 1  -- hoja nula: altura negra = 1
verificarAlturaNegra (Nodo color izq _ der) = do
    hIzq <- verificarAlturaNegra izq
    hDer <- verificarAlturaNegra der
    if hIzq /= hDer
        then Nothing  -- violación de la propiedad de altura negra
        else Just (hIzq + if color == Negro then 1 else 0)
