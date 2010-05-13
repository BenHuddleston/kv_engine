/* -*- Mode: C++; tab-width: 4; c-basic-offset: 4; indent-tabs-mode: nil -*- */
#ifndef ITEM_HH
#define ITEM_HH
#include "config.h"
#include "mutex.hh"
#include <string>
#include <string.h>
#include <stdio.h>
#include <memcached/engine.h>

/**
 * The Item structure we use to pass information between the memcached
 * core and the backend. Please note that the kvstore don't store these
 * objects, so we do have an extra layer of memory copying :(
 */
class Item {
public:
    Item(const void* k, const size_t nk, const size_t nb,
         const int fl, const rel_time_t exp, uint64_t theCas = 0) :
        flags(fl), exptime(exp), cas(theCas)
    {
        key.assign(static_cast<const char*>(k), nk);
        setData(NULL, nb);
    }

    Item(const std::string &k, const int fl, const rel_time_t exp,
         const void *dta, const size_t nb, uint64_t theCas = 0) :
        flags(fl), exptime(exp), cas(theCas)
    {
        key.assign(k);
        setData(static_cast<const char*>(dta), nb);
    }

    Item(const std::string &k, const int fl, const rel_time_t exp,
         const std::string &val, uint64_t theCas = 0) :
        flags(fl), exptime(exp), cas(theCas)
    {
        key.assign(k);
        setData(val.c_str(), val.size());
    }

    Item(const void *k, uint16_t nk, const int fl, const rel_time_t exp,
         const void *dta, const size_t nb, uint64_t theCas = 0) :
        flags(fl), exptime(exp), cas(theCas)
    {
        key.assign(static_cast<const char*>(k), nk);
        setData(static_cast<const char*>(dta), nb);
    }

    Item(const Item &itm) : flags(itm.flags), exptime(itm.exptime), cas(itm.cas)
    {
        key.assign(itm.key);
        setData(itm.data, itm.nbytes);
    }

    ~Item() {
        delete []data;
    }

    char *getData() {
        return data;
    }

    const std::string &getKey() const {
        return key;
    }

    int getNKey() const {
        return static_cast<int>(key.length());
    }

    uint32_t getNBytes() const {
        return nbytes;
    }

    rel_time_t getExptime() const {
        return exptime;
    }

    int getFlags() const {
        return flags;
    }

    uint64_t getCas() const {
        return cas;
    }

    void setCas() {
        cas = nextCas();
    }

    void setCas(uint64_t ncas) {
        cas = ncas;
    }

    /**
     * Append another item to this item
     *
     * @param item the item to append to this one
     * @return true if success
     */
    bool append(const Item &item) {
        uint32_t ns = nbytes + item.nbytes - 2;
        char *c = new char[ns];
        if (c == NULL) {
            return false;
        }

        memcpy(c, data, nbytes - 2);
        memcpy(c + nbytes - 2, item.data, item.nbytes);

        delete []data;
        data = c;
        nbytes = ns;

        return true;
    }

    /**
     * Prepend another item to this item
     *
     * @param item the item to prepend to this one
     * @return true if success
     */
    bool prepend(const Item &item) {
        uint32_t ns = nbytes + item.nbytes - 2;
        char *c = new char[ns];
        if (c == NULL) {
            return false;
        }

        memcpy(c, item.data, item.nbytes - 2);
        memcpy(c + item.nbytes - 2, data, nbytes);

        delete []data;
        data = c;
        nbytes = ns;

        return true;
    }

private:
    /**
     * Set the item's data. This is only used by constructors, so we
     * make it private.
     */
    void setData(const char *dta, const size_t nb) {
        nbytes = static_cast<uint32_t>(nb);

        if (dta != NULL) {
            if (nbytes < 2 || memcmp(dta + nb - 2, "\r\n", 2) != 0) {
                nbytes += 2;
            }
        }

        if (nbytes > 0) {
            data = new char[nbytes];
        } else {
            data = NULL;
        }

        if (data && dta) {
            memcpy(data, dta, nbytes);
            if (nb != nbytes) {
                memcpy(data + nb, "\r\n", 2);
            }
        }
    }

    int flags;
    rel_time_t exptime;
    uint32_t nbytes;
    std::string key;
    char *data;
    uint64_t cas;

    static uint64_t nextCas(void) {
        uint64_t ret;
        casMutex.aquire();
        ret = casCounter++;
        casMutex.release();
        if ((ret % casNotificationFrequency) == 0) {
            casNotifier(ret);
        }

        return ret;
    }

    static void initializeCas(uint64_t initial, void (*notifier)(uint64_t current), uint64_t frequency) {
        casCounter = initial;
        casNotifier = notifier;
        casNotificationFrequency = frequency;
    }

    static uint64_t casNotificationFrequency;
    static void (*casNotifier)(uint64_t);
    static uint64_t casCounter;
    static Mutex casMutex;
};

#endif
