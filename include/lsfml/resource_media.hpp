#ifndef LSFML_RESOURCE_MEDIA_HPP_INCLUDED
#define LSFML_RESOURCE_MEDIA_HPP_INCLUDED

#include <memory>

namespace lsfml {

template <typename Media, typename Res>
class resource_media: public Media
{
public:
    using res_ptr = std::shared_ptr<Res>;
    using res_cptr = std::shared_ptr<const Res>;

    // Constructors
    resource_media() = default;
    explicit resource_media(res_ptr const& resource)
        : Media(*resource)
        , m_resource(resource)
    {}
    explicit resource_media(res_ptr&& resource)
        : Media(*resource)
        , m_resource(std::move(resource))
    {}

    // Accessors
    res_ptr resource() { return m_resource; }
    res_cptr cresource() const { return m_resource; }
    void set_resource(res_ptr resource)
    {
        m_resource = std::move(resource);
        update_resource_media(*this, m_resource);
    }

private:
    res_ptr m_resource;
};

} // namespace lsfml

#endif // LSFML_RESOURCE_MEDIA_HPP_INCLUDED
