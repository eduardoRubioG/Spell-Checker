
        template<class HashedObj>
        Dictionary<HashedObj>::Dictionary(const HashedObj & notFound, int size) 
			: items(notFound, size)
	{}

        template<class HashedObj>
        void Dictionary<HashedObj>::insert(const HashedObj & key)
        {
 		items.insert(key);                       
        }

        template<class HashedObj>
	const HashedObj & Dictionary<HashedObj>::lookup (const HashedObj & key) const
	{
		items.find(key); 
	}

        template<class HashedObj>
	void Dictionary<HashedObj>::makeEmpty()
	{
		items.makeEmpty(); 
	}
