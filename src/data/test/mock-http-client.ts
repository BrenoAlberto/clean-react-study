import { HttpPostClient } from 'data/protocols/http/http-post-client'

export class HttpPostClientSpy implements HttpPostClient {
  url?: string

  post = async (url: string): Promise<void> => {
    this.url = url
    return await Promise.resolve()
  }
}
