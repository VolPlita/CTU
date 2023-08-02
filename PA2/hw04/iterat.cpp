class CMailIterator {
public:

    CMailIterator() {
        current = 0;
    }

    explicit CMailIterator(CMailList &boxFound) {
        box = boxFound;
        current = 0;
    }


    explicit operator bool() const {
        if (current < box.getTaken()) {
            return true;
        }
        return false;
    }

    bool operator!(void) const {
        if (current < box.getTaken()) {
            return false;
        }
        return true;
    }

    const CMail &operator*() const {
        return box[current];
    }

    CMailIterator &operator++() {
        current = current + 1;
        return *this;
    }

    [[nodiscard]] CMailList getBox() const{
        return box;
    }


    [[nodiscard]] CMail getCurrent() const {
        return box[current];
    }

private:
    CMailList box;
    size_t current;
};