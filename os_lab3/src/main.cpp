    #include <iostream>
    #include <thread>
    #include <vector>

    using namespace std;

    void min_elem(vector<int> const& elements, vector<int>& min, const int l, const int r, const int i) {
        int min_el = elements[l];

        for (int j = l; j <= r; ++j)
        {
            if (elements[j] < min_el)
                min_el = elements[j];
        }
    //    cout << this_thread::get_id() << endl;
        min[i] = min_el;
    }

    int main() {
        int th_n, el_n;
        cout << "Enter th_n: \n";
        cin >> th_n;
        cout << "Enter el_n and elements: \n";
        cin >> el_n;

        vector<int> elements(el_n);
        vector<int> min(th_n);
        vector<thread> threads(th_n);

        for (int i = 0; i < el_n; ++i)
            cin >> elements[i];

        int l = 0;
        int delta = el_n / th_n;
        int last_id = th_n - 1;

        for (int i = 0; i < th_n; ++i)
        {
            if (i != last_id)
            {
                threads[i] = thread(min_elem, ref(elements), ref(min), l, l + delta - 1, i);
                l += delta;
            } else
            {
                threads[i] = thread(min_elem, ref(elements), ref(min), l, el_n - 1, i);
            }
        }

        for (int i = 0; i < th_n; ++i)
        {
            threads[i].join();
        }

        for (int i = 0; i < th_n; ++i)
            cout << min[i] << ' ';

        cout << endl;

        int ans = min[0];
        for (int i = 1; i < th_n; ++i)
            if (min[i] < ans)
                ans = min[i];

        cout << ans << endl;
    }