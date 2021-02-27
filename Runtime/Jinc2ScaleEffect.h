#pragma once
#include "pch.h"
#include "GUIDs.h"
#include "Jinc2ScaleTransform.h"
#include "EffectBase.h"
#include <d2d1effecthelpers.h>


// Jinc2 �����㷨
// �ʺ�ͼ��Ŵ󣬿��ԽϺñ����Ե
class Jinc2ScaleEffect : public EffectBase {
public:
    IFACEMETHODIMP Initialize(
        _In_ ID2D1EffectContext* pEffectContext,
        _In_ ID2D1TransformGraph* pTransformGraph
    ) {
        HRESULT hr = Jinc2ScaleTransform::Create(pEffectContext, &_transform);
        if (FAILED(hr)) {
            return hr;
        }

        hr = pTransformGraph->SetSingleTransformNode(_transform.Get());
        if (FAILED(hr)) {
            return hr;
        }

        return S_OK;
    }

    HRESULT SetScale(D2D_VECTOR_2F value) {
        if (value.x <= 0 || value.y <= 0) {
            return E_INVALIDARG;
        }

        _transform->SetScale(value);
        return S_OK;
    }

    D2D_VECTOR_2F GetScale() const {
        return _transform->GetScale();
    }

    HRESULT SetWindowSinc(FLOAT value) {
        if (value <= 0) {
            return E_INVALIDARG;
        }

        _transform->SetWindowSinc(value);
        return S_OK;
    }

    FLOAT GetWindowSinc() const {
        return _transform->GetWindowSinc();
    }

    HRESULT SetSinc(FLOAT value) {
        if (value <= 0) {
            return E_INVALIDARG;
        }

        _transform->SetSinc(value);
        return S_OK;
    }

    FLOAT GetSinc() const {
        return _transform->GetSinc();
    }

    HRESULT SetARStrength(FLOAT value) {
        if (value < 0 || value > 1) {
            return E_INVALIDARG;
        }

        _transform->SetARStrength(value);
        return S_OK;
    }

    FLOAT GetARStrength() const {
        return _transform->GetARStrength();
    }

    enum PROPS {
        // ���ű�����Ĭ��ֵΪ (1,1)
        PROP_SCALE = 0,
        // �������0��ֵԽСͼ��Խ�����������о�ݡ�Ĭ��ֵΪ 0.5
        PROP_WINDOW_SINC = 1,
        // �������0��ֵԽ������Խ�����������ж�����Ĭ��ֵΪ 0.825
        PROP_SINC = 2,
        // ������ǿ�ȡ������� 0~1 ֮�䡣Ĭ��ֵΪ 0.5
        PROP_AR_STRENGTH = 3
    };

    static HRESULT Register(_In_ ID2D1Factory1* pFactory) {
        const D2D1_PROPERTY_BINDING bindings[] =
        {
            D2D1_VALUE_TYPE_BINDING(L"Scale", &SetScale, &GetScale),
            D2D1_VALUE_TYPE_BINDING(L"WindowSinc", &SetWindowSinc, &GetWindowSinc),
            D2D1_VALUE_TYPE_BINDING(L"Sinc", &SetSinc, &GetSinc),
            D2D1_VALUE_TYPE_BINDING(L"ARStrength", &SetARStrength, &GetARStrength)
        };

        HRESULT hr = pFactory->RegisterEffectFromString(CLSID_MAGPIE_JINC2_SCALE_EFFECT, XML(
            <?xml version='1.0'?>
            <Effect>
                <!--System Properties-->
                <Property name='DisplayName' type='string' value='Jinc2 Scale'/>
                <Property name='Author' type='string' value='Xu Liu'/>
                <Property name='Category' type='string' value='Scale'/>
                <Property name='Description' type='string' value='Jinc2 Scale'/>
                <Inputs>
                    <Input name='Source'/>
                </Inputs>
                <Property name='Scale' type='vector2'>
                    <Property name='DisplayName' type='string' value='Scale'/>
                    <Property name='Default' type='vector2' value='(1,1)'/>
                </Property>
                <Property name='WindowSinc' type = 'float'>
                    <Property name='DisplayName' type='string' value='WindowSinc' />
                    <Property name='Default' type='float' value='0.5' />
                </Property>
                <Property name='Sinc' type='float'>
                    <Property name='DisplayName' type='string' value='Sinc' />
                    <Property name='Default' type='float' value='0.825' />
                </Property>
                <Property name='ARStrength' type='float'>
                    <Property name='DisplayName' type='string' value='ARStrength' />
                    <Property name='Default' type='float' value = '0.5' />
                    <Property name='Min' type='float' value='0' />
                    <Property name='Max' type='float' value='1.0' />
                </Property>
            </Effect>
        ), bindings, ARRAYSIZE(bindings), CreateEffect);

        return hr;
    }

    static HRESULT CALLBACK CreateEffect(_Outptr_ IUnknown** ppEffectImpl) {
        *ppEffectImpl = static_cast<ID2D1EffectImpl*>(new Jinc2ScaleEffect());

        if (*ppEffectImpl == nullptr) {
            return E_OUTOFMEMORY;
        }

        return S_OK;
    }

private:
    Jinc2ScaleEffect() {}

    ComPtr<Jinc2ScaleTransform> _transform = nullptr;
};