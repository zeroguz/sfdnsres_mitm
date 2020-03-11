namespace util
{
    struct AddrInfo
    {
        u32 magic;
        int ai_flags;
        int ai_family;
        int ai_socktype;
        int ai_protocol;
        u32 ai_addrlen;
    };

} // namespace util