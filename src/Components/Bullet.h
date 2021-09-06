#pragma once

struct Bullet
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Bullet"
    };

	paperback::component::entity m_Owner;
};