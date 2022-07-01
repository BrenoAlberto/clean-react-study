import { HttpPostClient, HttpPostParams } from 'data/protocols/http/http-post-client'

export class HttpPostClientSpy implements HttpPostClient {
  url?: string

  post = async (params: HttpPostParams): Promise<void> => {
    this.url = params.url
    return await Promise.resolve()
  }
}
