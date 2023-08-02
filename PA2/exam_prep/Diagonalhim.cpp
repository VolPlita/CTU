template<typename T_>
class CDiagonalMatrix {
public:
    CDiagonalMatrix(int n, int k) : n(n), k(k) {
        if( k < 1 || k > n) throw out_of_range("invalid matrix size");
        // Prostřední diagonála
        for(int i = 0; i < n; i++){
            m_Diagonals.push_back({});
            m_Diagonals[0].push_back(T_());
        }

        // Diagonály okolo prostředku (Z obou stran v jednom vektoru). Vždycky jich bude (n-i)*2 (Násobíme dvakrát protože bereme diagonálu z obou stran)
        for(int i = 1; i < k; i++) {
            for(int l = 0; l < (n - i) * 2; l++){
                m_Diagonals[i].push_back(T_());
            }
        }
    }

    T_& operator()(int row, int col){
        // Vnější souřadnice je vpodstatě vzdálenost od prostředku
        int outer = abs(row - col);
        int inner;
        if(outer >= k) throw out_of_range("invalid index ( " + to_string(row) + ", " + to_string(col) + " )");

        // Vzhledem k tomu, že máme data uložená tak, že nejdřív máme diagonálu z pravé strany matice a pak z levé, tak když máme souřadnici z pravé strany matice, můžeme vzít rovnou řádek jakožto inner 
        // souřadnici. Když jsme na levé straně matice, tak musíme brát souřadnici jakoby z "pravé strany" vektoru, jako bychom od druhé půlky vektoru indexovali zase od nuly a toho docílíme tím, že si 
        // vydělíme délku vektoru dvěma a přičteme sloupec)
        if(col > row || col == row) {
            inner = row;
        }
        else {
            inner = col + (m_Diagonals[outer].size() / 2);
        }

        if(inner > col + (static_cast<int>(m_Diagonals[outer].size()) / 2)) throw out_of_range("invalid index ( " + to_string(row) + ", " + to_string(col) + " )");

        return m_Diagonals[outer][inner];
    }

    bool exists (int row, int col) const {
        int outer = abs(row - col);
        int inner;
        if(outer >= k) return false;
        if(col > row || col == row) {
            inner = row;
        }
        else {
            inner = col + (static_cast<int>(m_Diagonals[outer].size()) / 2);
        }

        if(inner > col + (static_cast<int>(m_Diagonals[outer].size()) / 2)) return false;

        return true;
    }

    void reshape ( int newK ) {
        if(newK > n) throw out_of_range("invalid matrix size");
        // Pokud je nové k menší než staré, zbavíme se přebývajících diagonál. Pokud je větší, alokujeme místo pro nové diagonály.
        if(newK < k) {
            m_Diagonals.erase(m_Diagonals.begin()+newK, m_Diagonals.begin()+k);
        }
        else if (newK > k){
            for(int i = k; i < newK; i++) {
                for(int l = 0; l < (n - i) * 2; l++){
                    m_Diagonals.push_back({});
                    m_Diagonals[i].push_back(T_());
                }
            }
        }
        k = newK;
    }

private:
    int n;
    int k;
    // Vektor vektorů k-ček
    vector<vector<T_>> m_Diagonals;
};